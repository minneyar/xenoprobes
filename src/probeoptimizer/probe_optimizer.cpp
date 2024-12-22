//
// Created by preed on 1/6/16.
//

#include <fstream>
#include <iostream>
#include <map>
// #include <bits/stl_algo.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <iomanip>
#include <mutex>
#include <ranges>
#include <spdlog/spdlog.h>
#include <thread>

#include "probeoptimizer/probe_optimizer.h"
#include "probeoptimizer/semaphore.h"
#include "probeoptimizer/solution.h"

#include <probeoptimizer/csv.h>

std::atomic<bool> ProbeOptimizer::shouldStop_(false);

ProbeOptimizer::ProbeOptimizer() {
  for (const auto &probeId : Probe::ALL | std::views::keys) {
    if (probeId == "B") {
      continue;
    }
    inventory_.emplace(Probe::fromString(probeId), 0);
  }
}

void ProbeOptimizer::loadInventory(const std::string &filename) {
  try {
    auto data = loadCSV(filename);
    loadInventory(data);
  } catch (const std::exception &e) {
    spdlog::error("Error loading {}: {}", filename, e.what());
  }
}

void ProbeOptimizer::loadInventory(
    const std::vector<std::vector<CsvRecordVal>> &records) {
  std::vector<std::pair<Probe::Id, unsigned int>> inventory;

  for (const auto &record : records) {
    try {
      const auto probeId = std::get<std::string>(record[0]);
      int num = csvRecordValToInt(record[1]);
      inventory.emplace_back(probeId, num);
    } catch (const std::exception &e) {
      spdlog::error("Bad inventory format: {}", e.what());
      throw;
    }
  }
  loadInventory(inventory);
}

void ProbeOptimizer::loadInventory(
    const std::vector<std::pair<Probe::Id, unsigned int>> &inventory) {
  ProbeInventory newInventory;

  for (const auto &[probeId, num] : inventory) {
    newInventory.emplace(Probe::fromString(probeId), num);
  }

  loadInventory(newInventory);
}

void ProbeOptimizer::loadInventory(const ProbeInventory &inventory) {
  ProbeInventory newInventory(inventory);

  // Ensure every probe type is accounted for in the inventory.
  ProbeInventory::mapped_type probeCount = 0;
  for (const auto &probeId : Probe::ALL | std::views::keys) {
    if (probeId == "B") {
      continue;
    }
    // Inserts a 0 if not already present in inventory.
    probeCount += newInventory[Probe::fromString(probeId)];
  }

  // ensure inventory is as big as needed to fill the entire map
  if (probeCount < sites_.size()) {
    newInventory[Probe::fromString("B")] = sites_.size() - probeCount;
  }

  spdlog::info("Inventory has {} probes.", probeCount);
  if (newInventory[Probe::fromString("B")] > 0)
    spdlog::info("{} are Basic Probes.", newInventory[Probe::fromString("B")]);
  inventory_ = newInventory;
}

void ProbeOptimizer::printInventory() const {
  for (auto &entry : inventory_)
    if (entry.first->category != Probe::Category::Basic)
      spdlog::info("{},{}", entry.first->id, entry.second);
}

void ProbeOptimizer::loadSetup(const std::string &filename) {
  const auto data = loadCSV(filename);
  std::unordered_map<Site::Id, Probe::Id> siteProbeMap;
  for (const auto &record : data) {
    try {
      const auto siteName = csvRecordValToInt(record[0]);
      siteProbeMap.emplace(siteName, std::get<std::string>(record[1]));
    } catch (const std::exception &e) {
      throw std::runtime_error("Setup file has bad format.");
    }
  }
  loadSetup(siteProbeMap);
}

void ProbeOptimizer::loadSetup(
    const std::unordered_map<Site::Id, Probe::Id> &siteProbeMap) {
  spdlog::info("Loading setup.");
  ProbeArrangement newSetup;
  newSetup.resize(sites_.size());

  for (const auto &[siteName, probeId] : siteProbeMap) {
    try {
      newSetup.setProbeAt(getIndexForSiteId(siteName),
                          Probe::fromString(probeId));
    } catch (const std::exception &e) {
      throw std::runtime_error{"Setup file uses a non-available site."};
    }
  }
  loadSetup(newSetup);
}

void ProbeOptimizer::loadSetup(const ProbeArrangement &setup) {
  setup_ = setup;
  solution_.setSetup(setup);
}

void ProbeOptimizer::loadSites(const std::string &filename) {
  try {
    auto data = loadCSV(filename);
    loadSites(data);
  } catch (std::exception &e) {
    spdlog::error("Error while loading {}: {}", filename, e.what());
    throw;
  }
}

void ProbeOptimizer::loadSites(
    const std::vector<std::vector<CsvRecordVal>> &records) {
  try {
    std::unordered_set<Site::Id> idList;

    for (const auto &record : records) {
      // Ignore all other columns in the list as those values are compiled in.
      const auto siteId = csvRecordValToInt(record[0]);
      idList.insert(siteId);
    }

    loadSites(idList);
  } catch (std::exception &e) {
    spdlog::error("Bad site data format: {}", e.what());
    throw;
  }
}

void ProbeOptimizer::loadSites(const std::unordered_set<Site::Id> &idList) {
  SiteList newSites;
  for (const auto id : idList) {
    try {
      newSites.insert(Site::fromName(id));
    } catch (std::out_of_range &) {
      spdlog::error("Could not find ID {}", idList.size());
      throw;
    }
  }
  loadSites(newSites);
}

void ProbeOptimizer::loadSites(const SiteList &sites) {
  int numConnections = 0;
  for (const auto site : sites) {
    numConnections += site->getNeighbors().size();
  }
  // Counted both edges of the connection.
  numConnections /= 2;
  spdlog::info("Loaded {} FN sites with {} connections.", sites.size(),
               numConnections);
  sites_ = sites;
  updateSiteListIndexes();
  setup_.resize(sites_.size());
}

void ProbeOptimizer::updateSiteListIndexes() {
  siteIdIndexMap_.clear();
  siteIndexIdMap_.clear();
  std::size_t ix = 0;
  for (const auto site : sites_) {
    siteIdIndexMap_.emplace(site->name, ix);
    siteIndexIdMap_.emplace(ix, site->name);
    ++ix;
  }
}

void ProbeOptimizer::printSetup() const { setup_.printSetup(); }

void ProbeOptimizer::printTotals() const { setup_.printTotals(); }

void ProbeOptimizer::doHillClimbing(ProgressCallback progressCallback,
                                    StopCallback stopCallback) const {
  spdlog::info(
      "Starting Hill Climbing with parameters:\n"
      "  storage weight   = {storageWeight: 6}\n"
      "  revenue weight   = {revenueWeight: 6}\n"
      "  production weight= {productionWeight: 6}\n"
      "  iterations={maxIterations}  offsprings={numOffsprings}"
      "  mutation={mutationRate}  age={maxAge}  population={maxPopSize}",
      fmt::arg("storageWeight", setup_.getStorageWeight()),
      fmt::arg("revenueWeight", setup_.getRevenueWeight()),
      fmt::arg("productionWeight", setup_.getProductionWeight()),
      fmt::arg("maxIterations", maxIterations_),
      fmt::arg("numOffsprings", numOffsprings_),
      fmt::arg("mutationRate", mutationRate_), fmt::arg("maxAge", maxAge_),
      fmt::arg("maxPopSize", maxPopSize_));

  std::vector<Solution> population(maxPopSize_), newGen;
  for (auto &solution : population) {
    solution.randomize();
    solution.evaluate();
  }

  Solution best, worst;
  size_t killed = 0;
  for (size_t iter = 0; iter < maxIterations_ && !stopCallback(); ++iter) {
    spdlog::info(
        "iteration: {iter}/{maxIterations}  best: {best:.9}  worst: {worst:.9} "
        " killed {killed}",
        fmt::arg("iter", iter + 1), fmt::arg("maxIterations", maxIterations_),
        fmt::arg("best", best.getScore()), fmt::arg("worst", worst.getScore()),
        fmt::arg("killed", killed));
    if (progressCallback) {
      progressCallback(iter + 1, best.getScore(), worst.getScore(), killed);
    }

    newGen.clear();
    killed = 0;

    std::vector<std::thread> threads;
    std::mutex newGenLock;
    int count = 0;
    semaphore sem(max_threads_);
    for (const auto &ancestor : population) {
      int local_count = count;
      sem.wait();
      auto functor = [&] {
        // First, create a bunch of children of this and try t
        // find the best one.
        auto bestChild = ancestor.findBestChild(numOffsprings_, mutationRate_);
        // If none of the children were better than the original,
        // this generation's a flop.  Reset it from scratch if it's old
        // Don't reset it if it's the best one we've gotten so far, though.
        if (bestChild.getScore() == ancestor.getScore() && maxAge_ > 0 &&
            bestChild.getScore() != solution_.getScore()) {
          bestChild.setAge(bestChild.getAge() + 1);
          if (bestChild.getAge() > maxAge_) {
            bestChild.randomize();
            bestChild.evaluate();
            bestChild.setAge(0);
            ++killed;
          }
        }

        {
          std::unique_lock<std::mutex> lock(newGenLock);
          newGen.push_back(std::move(bestChild));
        }
        sem.notify();
      };
      threads.push_back(std::thread(functor));
    }
    for (auto &thread : threads) {
      thread.join();
    }
    threads.clear();

    auto extremes = std::minmax_element(newGen.begin(), newGen.end());
    worst = *extremes.first;
    best = *extremes.second;

    if (best > solution_) {
      solution_ = best;
    }

    swap(population, newGen);
  }

  solution_.printSetup();
  solution_.printTotals();
  spdlog::info("# Best score: {: 9}", solution_.getScore());
}

void ProbeOptimizer::setStorageWeight(float storageWeight) {
  setup_.setStorageWeight(storageWeight);
}

void ProbeOptimizer::setRevenueWeight(float revenueWeight) {
  setup_.setRevenueWeight(revenueWeight);
}

void ProbeOptimizer::setProductionWeight(float productionWeight) {
  setup_.setProductionWeight(productionWeight);
}

void ProbeOptimizer::setMutationRate(float mutationRate) {
  mutationRate_ = mutationRate;
}

void ProbeOptimizer::setMaxPopSize(size_t maxPopSize) {
  maxPopSize_ = maxPopSize;
}

void ProbeOptimizer::setNumOffsprings(size_t numOffsprings) {
  numOffsprings_ = numOffsprings;
}

void ProbeOptimizer::setMaxIterations(size_t maxIterations) {
  maxIterations_ = maxIterations;
}

void ProbeOptimizer::setMaxAge(int maxAge) { maxAge_ = maxAge; }

void ProbeOptimizer::setMaxThreads(size_t threads) { max_threads_ = threads; }

void ProbeOptimizer::handleSIGINT(int) { requestStop(); }

void ProbeOptimizer::requestStop() { shouldStop_ = true; }

const ProbeArrangement &ProbeOptimizer::getDefaultArrangement() {
  return setup_;
}

ProbeOptimizer::SiteList &ProbeOptimizer::getSites() { return sites_; }

std::size_t ProbeOptimizer::getIndexForSiteId(Site::Id siteId) {
  try {
    return siteIdIndexMap_.at(siteId);
  } catch (std::out_of_range &e) {
    spdlog::error("No index for site id {}", siteId);
    throw;
  }
}

Site::Id ProbeOptimizer::getSiteIdForIndex(std::size_t index) {
  try {
    return siteIndexIdMap_.at(index);
  } catch (std::out_of_range &e) {
    spdlog::error("No site id for index {}", index);
    throw;
  }
}

ProbeOptimizer::ProbeInventory &ProbeOptimizer::getInventory() {
  return inventory_;
}
