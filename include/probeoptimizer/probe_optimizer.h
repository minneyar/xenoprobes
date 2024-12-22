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
  using ProbeInventory = std::map<Probe::Ptr, unsigned int>;
  using ProgressCallback =
      std::function<void(unsigned long iter, double bestScore,
                         double worstScore, unsigned long killed)>;
  using StopCallback = std::function<bool()>;

  void loadInventory(const std::string &filename);
  void loadInventory(const std::vector<std::vector<CsvRecordVal>> &records);
  void loadInventory(
      const std::vector<std::pair<Probe::Id, unsigned int>> &inventory);
  void loadInventory(const ProbeInventory &inventory);
  void loadSetup(const std::string &filename);
  template <typename T> void loadSites(T sites) {
    sites_ = loadSiteList(sites);
    updateSiteListIndexes();
  }

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
  static const std::vector<Site::Ptr> &getSites();
  static std::size_t getIndexForSiteId(Site::Id siteId);
  static ProbeInventory &getInventory();
  [[nodiscard]] const Solution &solution() const { return solution_; }

  bool isValid() const {
    ProbeInventory histoSetup;

    for (size_t i = 0; i < setup_.getSize(); i++) {
      ++histoSetup[setup_.getProbeAt(i)];
    }
    return inventory_ == histoSetup;
  }

private:
  inline static ProbeInventory inventory_;
  inline static std::vector<Site::Ptr> sites_;
  // Needed because many operations require indexes in sites_ and cannot be
  // converted to use a map because of their nature.
  inline static std::unordered_map<Site::Id, std::size_t> siteListIndexes_;
  inline static ProbeArrangement setup_;
  static void updateSiteListIndexes();

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
