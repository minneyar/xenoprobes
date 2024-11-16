//
// Created by preed on 1/6/16.
//

#include <map>
#include <iostream>
#include <fstream>
// #include <bits/stl_algo.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <iomanip>
#include <thread>
#include <mutex>

#include "probeoptimizer/probe_optimizer.h"
#include "probeoptimizer/solution.h"
#include "probeoptimizer/semaphore.h"

std::atomic<bool> ProbeOptimizer::shouldStop_(false);
ProbeArrangement ProbeOptimizer::setup_;
SiteList ProbeOptimizer::sites_;
std::vector<Probe::Type> ProbeOptimizer::inventory_;

void ProbeOptimizer::loadInventory()
{
    inventory_.clear();

    try {
        auto data = loadCSV(inventoryInput_);
        for (const auto& record : data) {
            int num = std::stoi(record[1]);
            for (int i=0; i<num; ++i)
                inventory_.push_back(Probe::fromString(record[0]));
        }

        // ensure inventory is as big as needed to fill the entire map
        while (inventory_.size() < sites_.size())
            inventory_.push_back(Probe::Type::Basic);

        std::clog << "Inventory has " << inventory_.size() << " probes";
        auto numBasic = std::count(inventory_.begin(), inventory_.end(), Probe::Type::Basic);
        if (numBasic > 0)
            std::clog << ", and " << numBasic << " are Probe::Type::Basic Probes." << std::endl;
        else
            std::clog << "." << std::endl;
    }
    catch (std::exception& e) {
        throw std::runtime_error{"Loading " + inventoryInput_ + std::string(" inventory: ") + e.what()};
    }
}


void ProbeOptimizer::printInventory() const
{
    std::map< Probe::Type, int> histo;
    for (auto item : inventory_)
        ++histo[item];
    for (auto& entry : histo)
        if (entry.first != Probe::Type::Basic)
            std::cout << Probe::toString(entry.first) << "," << entry.second << std::endl;
}

void ProbeOptimizer::loadSetup()
{
    std::cout << "Loading setup." << std::endl;
    setup_.resize(sites_.size());

    auto data = loadCSV(setupInput_);
    for (auto& record : data) {
        int siteName = std::stoi(record[0]);
        int siteIdx = sites_.findIndexForSiteName(siteName);
        if (siteIdx == -1) {
            throw std::runtime_error{"Setup file uses a non-available site: " + std::to_string(siteIdx)};
        }
        setup_.setProbeAt(siteIdx, Probe::fromString(record[1]));
    }
}

void ProbeOptimizer::loadSites() {
    sites_.loadSites();
}

std::vector<std::vector<std::string>> ProbeOptimizer::loadCSV(const std::string& fname)
{
    using std::vector;
    using std::string;

    vector<vector<string>> result;

    std::ifstream file{fname};
    if (!file) {
        throw std::runtime_error{"could not open file " + fname};
    }
    string line;
    while (getline(file, line)) {
        auto commentPos = line.find('#');
        if (commentPos != string::npos)
            line.erase(commentPos);
        boost::trim(line);
        if (line.empty())
            continue;
        vector<string> records;
        boost::split(records, line, [](char c) {return c==',';});
        result.push_back(std::move(records));
    }

    return result;
}

void ProbeOptimizer::printSetup() const {
    setup_.printSetup();
}

void ProbeOptimizer::printTotals() const {
    setup_.printTotals();
}

void ProbeOptimizer::doHillClimbing() const
{
    std::clog << "Starting Hill Climbing with parameters:" << std::endl;
    std::clog << "  storage weight   = " << std::setw(6) << setup_.getStorageWeight() << std::endl
    << "  revenue weight   = " << std::setw(6) << setup_.getRevenueWeight() << std::endl
    << "  production weight= " << std::setw(6) << setup_.getProductionWeight() << std::endl;
    std::clog << "  iterations=" << maxIterations_ << "  offsprings=" << numOffsprings_
    << "  mutation=" << mutationRate_ << "  age=" << maxAge_
    << "  population=" << maxPopSize_ << std::endl;


    std::vector<Solution> population(maxPopSize_), newGen;
    for (auto& solution : population) {
        solution.randomize();
        solution.evaluate();
    }

    Solution globalBest, best, worst;
    size_t killed = 0;
    for (size_t iter=0; iter<maxIterations_ && !shouldStop_; ++iter) {

        std::clog << "iteration: " << (iter+1) << "/" << maxIterations_
        << "  best: " << std::setprecision(9) << best.getScore()
        << "  worst: " << std::setprecision(9) << worst.getScore()
        << "  killed: " << killed
        << std::endl;

        newGen.clear();
        killed = 0;

        std::vector<std::thread> threads;
        std::mutex newGenLock;
        int count = 0;
        semaphore sem(max_threads_);
        for (const auto& ancestor : population) {
            int local_count = count;
            sem.wait();
            auto functor = [&] {
              // First, create a bunch of children of this and try t
              // find the best one.
              auto bestChild = ancestor.findBestChild(numOffsprings_, mutationRate_);
              // If none of the children were better than the original,
              // this generation's a flop.  Reset it from scratch if it's old
              // Don't reset it if it's the best one we've gotten so far, though.
              if (bestChild.getScore() == ancestor.getScore() &&
                  maxAge_ > 0 &&
                  bestChild.getScore() != globalBest.getScore() ) {
                  bestChild.setAge(bestChild.getAge()+1);
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
        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();

        auto extremes = std::minmax_element(newGen.begin(), newGen.end());
        worst = *extremes.first;
        best = *extremes.second;

        if (best > globalBest) {
            globalBest = best;
        }

        swap(population, newGen);
    }
    if (shouldStop_) {
        std::clog << std::endl; // break line with ^C character
    }

    globalBest.printSetup();
    globalBest.printTotals();
    std::cout << "# Best score: " << std::setprecision(9) << globalBest.getScore() << std::endl;
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

void ProbeOptimizer::setMaxAge(int maxAge) {
    maxAge_ = maxAge;
}

void ProbeOptimizer::setSetupInput(const std::string &setupInput) {
    setupInput_ = setupInput;
}

void ProbeOptimizer::setInventoryInput(const std::string &inventoryInput) {
    inventoryInput_ = inventoryInput;
}

void ProbeOptimizer::setMaxThreads(size_t threads)
{
    max_threads_ = threads;
}

void ProbeOptimizer::handleSIGINT(int) {
    shouldStop_ = true;
}

const ProbeArrangement& ProbeOptimizer::getDefaultArrangement() {
    return setup_;
}

const SiteList& ProbeOptimizer::getSites() {
    return sites_;
}

std::vector<Probe::Type> ProbeOptimizer::getInventory() {
    return inventory_;
}
