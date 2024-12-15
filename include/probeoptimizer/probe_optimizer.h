//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_PROBE_OPTIMIZER_H
#define XENOPROBES_PROBE_OPTIMIZER_H

#include "probe.h"
#include "probe_arrangement.h"
#include "site_list.h"
#include "solution.h"
#include <atomic>
#include <functional>
#include <map>
#include <vector>

class ProbeOptimizer {
public:
  using ProgressCallback =
      std::function<void(unsigned long iter, double bestScore,
                         double worstScore, unsigned long killed)>;
  using StopCallback = std::function<bool()>;

  void loadInventory(const std::string &filename);
  void loadInventory(const std::vector<std::vector<CsvRecordVal>> &records);
  void loadSetup(const std::string &filename);
  void loadSites(const std::string &filename);
  void loadSites(const std::vector<std::vector<CsvRecordVal>> &records);

  void printInventory() const;
  void printSetup() const;
  void printTotals() const;

  void
  doHillClimbing(ProgressCallback progressCallback = {},
                 StopCallback stopCallback = &ProbeOptimizer::shouldStop) const;

  void setStorageWeight(float storageWeight);
  void setRevenueWeight(float revenueWeight);
  void setProductionWeight(float productionWeight);
  void setMutationRate(float mutationRate);
  void setMaxPopSize(size_t maxPopSize);
  void setNumOffsprings(size_t numOffsprings);
  void setMaxIterations(size_t maxIterations);
  void setMaxAge(int maxAge);
  void setMaxThreads(size_t threads);

  static void handleSIGINT(int);
  static void requestStop();
  static bool shouldStop() { return shouldStop_; }

  static const ProbeArrangement &getDefaultArrangement();
  static const SiteList &getSites();
  static std::vector<Probe::Ptr> getInventory();
  [[nodiscard]] const Solution &solution() const { return solution_; }

  bool isValid() const {
    std::map<Probe::Ptr, int> histoInv;
    std::map<Probe::Ptr, int> histoSetup;

    for (const auto item : inventory_) {
      ++histoInv[item];
    }
    for (size_t i = 0; i < setup_.getSize(); i++) {
      ++histoSetup[setup_.getProbeAt(i)];
    }
    return histoInv == histoSetup;
  }

private:
  inline static std::vector<Probe::Ptr> inventory_;
  inline static SiteList sites_;
  inline static ProbeArrangement setup_;

  float mutationRate_;
  float eliteRatio_;
  size_t maxPopSize_;
  size_t tournamentRank_;
  size_t numOffsprings_;
  size_t maxIterations_;
  int maxAge_;
  size_t max_threads_;
  mutable Solution solution_;

  static std::atomic<bool> shouldStop_;
};

#endif // XENOPROBES_PROBE_OPTIMIZER_H
