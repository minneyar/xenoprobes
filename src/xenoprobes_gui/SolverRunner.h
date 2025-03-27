/**
 * @file SolverRunner.h
 *
 * @author Dan Keenan
 * @date 11/30/24
 * @copyright GNU GPLv3
 */

#ifndef SOLVERRUNNER_H
#define SOLVERRUNNER_H

#include "DataProbe.h"
#include "MiraMap.h"
#include "RunOptions.h"

#include <QThread>
#include <probeoptimizer/probe_optimizer.h>

class SolverRunner : public QThread {
  Q_OBJECT
public:
  explicit SolverRunner(ProbeOptimizer *probeOptimizer,
                        const RunOptions &runOptions,
                        QObject *parent = nullptr);

Q_SIGNALS:
  void progress(unsigned long iter, double bestScore, double worstScore,
                unsigned long killed);
  void finished(ProbeArrangement probeArrangement);

protected:
  void run() override;

private:
  ProbeOptimizer *probeOptimizer_;
  RunOptions runOptions_;
};

#endif // SOLVERRUNNER_H
