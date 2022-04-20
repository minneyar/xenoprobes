//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_PROBE_OPTIMIZER_H
#define XENOPROBES_PROBE_OPTIMIZER_H

#include <vector>
#include <atomic>
#include <map>
#include "site_list.h"
#include "probe.h"
#include "probe_arrangement.h"
#include "solution.h"

class ProbeOptimizer {
public:
    void loadInventory();
    void loadSetup();
    void loadSites();

    void printInventory() const;
    void printSetup() const;
    void printTotals() const;

    static std::vector<std::vector<std::string>> loadCSV(const std::string& fname);

    void doHillClimbing() const;

    void setStorageWeight(float storageWeight);
    void setRevenueWeight(float revenueWeight);
    void setProductionWeight(float productionWeight);
    void setMutationRate(float mutationRate);
    void setMaxPopSize(size_t maxPopSize);
    void setNumOffsprings(size_t numOffsprings);
    void setMaxIterations(size_t maxIterations);
    void setMaxAge(int maxAge);
    void setSetupInput(const std::string &setupInput);
    void setInventoryInput(const std::string &inventoryInput);
    void setMaxThreads(size_t threads);

    static void handleSIGINT(int);

    static const ProbeArrangement& getDefaultArrangement();
    static const SiteList& getSites();
    static std::vector<Probe::Type> getInventory();

    bool isValid() const
    {
        std::map< Probe::Type,int> histoInv;
        std::map< Probe::Type,int> histoSetup;

        for (const auto& item : inventory_) {
            ++histoInv[item];
        }
        for (size_t i = 0; i < setup_.getSize(); i++) {
            ++histoSetup[setup_.getProbeAt(i)];
        }
        return histoInv == histoSetup;
    }

private:
    static std::vector<Probe::Type> inventory_;
    static SiteList sites_;
    static ProbeArrangement setup_;

    float mutationRate_;
    float eliteRatio_;
    size_t maxPopSize_;
    size_t tournamentRank_;
    size_t numOffsprings_;
    size_t maxIterations_;
    int maxAge_;
    std::string setupInput_;
    std::string inventoryInput_;
    size_t max_threads_;

    static std::atomic<bool> shouldStop_;
};


#endif //XENOPROBES_PROBE_OPTIMIZER_H
