/**
 * @file SolverRunner.cpp
 *
 * @author Dan Keenan
 * @date 11/30/24
 * @copyright GNU GPLv3
 */

#include "SolverRunner.h"

#include <probeoptimizer/probe_optimizer.h>

SolverRunner::SolverRunner(const FnSite::IdList &siteList,
                           const MiraMap::SiteProbeMap &siteProbeMap,
                           const DataProbe::ProbeInventory &probeInventory,
                           const RunOptions &runOptions, QObject *parent)
    : QThread(parent), siteList_(siteList), siteProbeMap_(siteProbeMap),
      probeInventory_(probeInventory), runOptions_(runOptions) {}

void SolverRunner::run() {
  ProbeOptimizer optimizer;
  optimizer.setStorageWeight(runOptions_.storageWeight);
  optimizer.setRevenueWeight(runOptions_.revenueWeight);
  optimizer.setProductionWeight(runOptions_.productionWeight);
  optimizer.setMaxIterations(runOptions_.iterations);
  optimizer.setNumOffsprings(runOptions_.offsprings);
  optimizer.setMutationRate(runOptions_.mutation / 100.0);
  optimizer.setMaxPopSize(runOptions_.population);

  std::vector<std::vector<CsvRecordVal>> records;
  // Load sites.
  records.reserve(siteList_.size());
  for (const auto &siteId : siteList_) {
    const auto &site = FnSite::kAllSites.value(siteId);
    auto &record = records.emplace_back();
    record.reserve(8);
    record.emplace_back(static_cast<int>(siteId));
    record.emplace_back(std::string(1, FnSite::gradeToChar(site.miningGrade)));
    record.emplace_back(std::string(1, FnSite::gradeToChar(site.revenueGrade)));
    record.emplace_back(std::string(1, FnSite::gradeToChar(site.combatGrade)));
    record.emplace_back(static_cast<int>(site.sightseeingSpots));
    for (const auto &ore : site.ore) {
      record.emplace_back(ore.toStdString());
    }
  }
  optimizer.loadSites(records);
  records.clear();

  // Load inventory.
  records.reserve(probeInventory_.size());
  for (const auto &[probeId, quantity] : probeInventory_) {
    if (quantity == 0) {
      continue;
    }
    auto &record = records.emplace_back();
    record.reserve(2);
    record.emplace_back(probeId.toStdString());
    record.emplace_back(static_cast<int>(quantity));
  }
  optimizer.loadInventory(records);
  records.clear();

  // Do the thing.
  optimizer.doHillClimbing(
      [this](unsigned long iter, double bestScore, double worstScore,
             unsigned long killed) {
        Q_EMIT(progress(iter, bestScore, worstScore, killed));
      },
      [this]() { return isInterruptionRequested(); });

  // Report solution.
  const auto &solutionSetup = optimizer.solution().getSetup();
  QStringList ores;
  for (const auto &ore : solutionSetup.getOres()) {
    ores.push_back(QString::fromStdString(ore));
  }
  for (const auto [siteId, probe] : solutionSetup.getSetup()) {
    siteProbeMap_[siteId] = QString::fromStdString(probe->id);
  }
  Q_EMIT(finished(solutionSetup.getTotalProduction(),
                  solutionSetup.getTotalRevenue(),
                  solutionSetup.getTotalStorage(), ores, siteProbeMap_));
}
