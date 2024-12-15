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
#include <spdlog/spdlog.h>
#include <thread>

#include "probeoptimizer/probe_optimizer.h"
#include "probeoptimizer/semaphore.h"
#include "probeoptimizer/solution.h"

#include <probeoptimizer/csv.h>

std::atomic<bool> ProbeOptimizer::shouldStop_(false);

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
  inventory_.clear();

  for (const auto &record : records) {
    int num = csvRecordValToInt(record[1]);
    for (int i = 0; i < num; ++i)
      inventory_.push_back(Probe::fromString(std::get<std::string>(record[0])));
  }

  // ensure inventory is as big as needed to fill the entire map
  inventory_.reserve(sites_.size());
  while (inventory_.size() < sites_.size())
    inventory_.push_back(Probe::fromString("B"));

  spdlog::info("Inventory has {} probes.", inventory_.size());
  auto numBasic = std::count_if(
      inventory_.begin(), inventory_.end(), [](const Probe::Ptr probe) {
        return probe->category == Probe::Category::Basic;
      });
  if (numBasic > 0)
    spdlog::info("{} are Basic Probes.", numBasic);
}

void ProbeOptimizer::printInventory() const {
  std::map<Probe::Ptr, int> histo;
  for (auto item : inventory_)
    ++histo[item];
  for (auto &entry : histo)
    if (entry.first->category != Probe::Category::Basic)
      spdlog::info("{},{}", entry.first->id, entry.second);
}

void ProbeOptimizer::loadSetup(const std::string &filename) {
  spdlog::info("Loading setup.");
  setup_.resize(sites_.size());

  const auto data = loadCSV(filename);
  for (const auto &record : data) {
    const auto siteName = csvRecordValToInt(record[0]);
    try {
      const auto site = Site::fromName(siteName);
      setup_.setProbeAt(site->name,
                        Probe::fromString(std::get<std::string>(record[1])));
    } catch (const std::exception &e) {
      throw std::runtime_error{"Setup file uses a non-available site."};
    }
  }
}

void ProbeOptimizer::loadSites(const std::string &filename) {
  sites_ = loadSiteList(filename);
  updateSiteListIndexes();
}

void ProbeOptimizer::loadSites(
    const std::vector<std::vector<CsvRecordVal>> &records) {
  sites_ = loadSiteList(records);
  updateSiteListIndexes();
}

void ProbeOptimizer::updateSiteListIndexes() {
  siteListIndexes.clear();
  siteListIndexes.reserve(sites_.size());
  for (std::size_t ix = 0; ix < sites_.size(); ++ix) {
    const auto site = sites_.at(ix);
    siteListIndexes.emplace(site->name, ix);
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

const std::vector<Site::Ptr> &ProbeOptimizer::getSites() { return sites_; }

std::size_t ProbeOptimizer::getIndexForSiteId(Site::Id siteId) {
  return siteListIndexes.at(siteId);
}

std::vector<Probe::Ptr> ProbeOptimizer::getInventory() { return inventory_; }
