/**
 * @file SolverRunner.cpp
 *
 * @author Dan Keenan
 * @date 11/30/24
 * @copyright GNU GPLv3
 */

#include "SolverRunner.h"

#include <probeoptimizer/probe_optimizer.h>

SolverRunner::SolverRunner(ProbeOptimizer *probeOptimizer,
                           const std::unordered_set<Site::Id> &siteList,
                           const MiraMap::SiteProbeMap &siteProbeMap,
                           const RunOptions &runOptions, QObject *parent)
    : QThread(parent), probeOptimizer_(probeOptimizer), siteList_(siteList),
      siteProbeMap_(siteProbeMap), runOptions_(runOptions) {}

void SolverRunner::run() {
  probeOptimizer_->setStorageWeight(runOptions_.storageWeight);
  probeOptimizer_->setRevenueWeight(runOptions_.revenueWeight);
  probeOptimizer_->setProductionWeight(runOptions_.productionWeight);
  probeOptimizer_->setMaxIterations(runOptions_.iterations);
  probeOptimizer_->setNumOffsprings(runOptions_.offsprings);
  probeOptimizer_->setMutationRate(runOptions_.mutation / 100.0);
  probeOptimizer_->setMaxPopSize(runOptions_.population);

  // Load enabled sites.
  probeOptimizer_->loadSites(siteList_);

  // Do the thing.
  probeOptimizer_->doHillClimbing(
      [this](unsigned long iter, double bestScore, double worstScore,
             unsigned long killed) {
        Q_EMIT(progress(iter, bestScore, worstScore, killed));
      },
      [this]() { return isInterruptionRequested(); });

  // Report solution.
  const auto &solutionSetup = probeOptimizer_->solution().getSetup();
  QStringList ores;
  for (const auto &ore : solutionSetup.getOres()) {
    ores.push_back(QString::fromStdString(ore));
  }
  for (const auto [site, probe] : solutionSetup.getSetup()) {
    siteProbeMap_[site->name] = probe->id;
  }
  Q_EMIT(finished(solutionSetup.getTotalProduction(),
                  solutionSetup.getTotalRevenue(),
                  solutionSetup.getTotalStorage(), ores, siteProbeMap_));
}
