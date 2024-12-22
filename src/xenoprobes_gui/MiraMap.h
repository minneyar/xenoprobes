/**
 * @file MiraMap.h
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#ifndef MIRAMAP_H
#define MIRAMAP_H

#include <QGraphicsView>

#include "FnSiteWidget.h"
#include "QGraphicsItemDeleter.h"

#include <probeoptimizer/probe_optimizer.h>
#include <probeoptimizer/site.h>
#include <unordered_set>

class MiraMap : public QGraphicsView {
  Q_OBJECT

public:
  explicit MiraMap(ProbeOptimizer *probeOptimizer, QWidget *parent = nullptr);
  void setProbeOptimizer(ProbeOptimizer *probeOptimizer);
  void setViewMode(const FnSiteWidget::ViewMode viewMode);

Q_SIGNALS:
  void sitesVisitedChanged();
  void siteProbeMapChanged();

protected:
  void wheelEvent(QWheelEvent *event) override;

private:
  static constexpr auto kZSites = 0;
  static constexpr auto kZLinks = -10;
  static constexpr auto kZMap = -100;
  QGraphicsScene mapScene_;
  ProbeOptimizer *probeOptimizer_;
  // std::unordered_set<Site::Id> sitesVisited_;
  std::vector<std::unique_ptr<QGraphicsItem, QGraphicsItemDeleter>>
      siteWidgets_;
  std::vector<std::unique_ptr<QGraphicsItem, QGraphicsItemDeleter>>
      linkGraphics_;

private Q_SLOTS:
  void calculateSiteWidgets();
  void calculateLinks();
};

#endif // MIRAMAP_H
