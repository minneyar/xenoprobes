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
                           const RunOptions &runOptions, QObject *parent)
    : QThread(parent), probeOptimizer_(probeOptimizer),
      runOptions_(runOptions) {}

void SolverRunner::run() {
  probeOptimizer_->setStorageWeight(runOptions_.storageWeight);
  probeOptimizer_->setRevenueWeight(runOptions_.revenueWeight);
  probeOptimizer_->setProductionWeight(runOptions_.productionWeight);
  probeOptimizer_->setMaxIterations(runOptions_.iterations);
  probeOptimizer_->setNumOffsprings(runOptions_.offsprings);
  probeOptimizer_->setMutationRate(runOptions_.mutation / 100.0);
  probeOptimizer_->setMaxPopSize(runOptions_.population);
  probeOptimizer_->setMaxThreads(runOptions_.threads);

  // Do the thing.
  probeOptimizer_->doHillClimbing(
      [this](unsigned long iter, double bestScore, double worstScore,
             unsigned long killed) {
        Q_EMIT(progress(iter, bestScore, worstScore, killed));
      },
      [this]() { return isInterruptionRequested(); });

  // Report solution.
  Q_EMIT(finished(probeOptimizer_->solution().getSetup()));
}
