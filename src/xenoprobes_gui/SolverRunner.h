/**
 * @file SolverRunner.h
 *
 * @author Dan Keenan
 * @date 11/30/24
 * @copyright GNU GPLv3
 */

#ifndef SOLVERRUNNER_H
#define SOLVERRUNNER_H

#include "FnSite.h"
#include "MiraMap.h"
#include "RunOptions.h"

#include <QThread>

class SolverRunner : public QThread {
  Q_OBJECT
public:
  explicit SolverRunner(const FnSite::IdList &siteList,
                        const MiraMap::SiteProbeMap &siteProbeMap,
                        const DataProbe::ProbeInventory &probeInventory,
                        const RunOptions &runOptions,
                        QObject *parent = nullptr);

Q_SIGNALS:
  void progress(unsigned long iter, double bestScore, double worstScore,
                unsigned long killed);
  void finished(unsigned int mining, unsigned int revenue, unsigned int storage,
                QStringList ores, MiraMap::SiteProbeMap siteProbeMap);

protected:
  void run() override;

private:
  FnSite::IdList siteList_;
  MiraMap::SiteProbeMap siteProbeMap_;
  DataProbe::ProbeInventory probeInventory_;
  RunOptions runOptions_;
};

#endif // SOLVERRUNNER_H
