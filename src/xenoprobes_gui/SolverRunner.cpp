/**
 * @file SolverRunner.cpp
 *
 * @author Dan Keenan
 * @date 11/30/24
 * @copyright GNU GPLv3
 */

#include "SolverRunner.h"

#include <probeoptimizer/probe_optimizer.h>

SolverRunner::SolverRunner(const std::unordered_set<Site::Id> &siteList,
                           const MiraMap::SiteProbeMap &siteProbeMap,
                           const ProbeInventory &probeInventory,
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

  // Load enabled sites.
  optimizer.loadSites(siteList_);

  // Load inventory.
  optimizer.loadInventory(probeInventory_);

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
    siteProbeMap_[siteId] = probe->id;
  }
  Q_EMIT(finished(solutionSetup.getTotalProduction(),
                  solutionSetup.getTotalRevenue(),
                  solutionSetup.getTotalStorage(), ores, siteProbeMap_));
}
