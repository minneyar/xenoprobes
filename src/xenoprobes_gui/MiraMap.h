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

class MiraMap : public QGraphicsView {
  Q_OBJECT

public:
  explicit MiraMap(ProbeOptimizer *probeOptimizer, QWidget *parent = nullptr);
  void setProbeOptimizer(ProbeOptimizer *probeOptimizer);
  void setViewMode(const FnSiteWidget::ViewMode viewMode);
  void fitAll();

Q_SIGNALS:
  void sitesVisitedChanged();
  void siteProbeMapChanged();

protected:
  void resizeEvent(QResizeEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
  void probeContextMenuEvent(QContextMenuEvent *event);

private:
  using GraphicsItemPtr = std::unique_ptr<QGraphicsItem, QGraphicsItemDeleter>;
  static constexpr auto kZSites = 0;
  static constexpr auto kZCombo = -5;
  static constexpr auto kZLinks = -10;
  static constexpr auto kZMap = -100;
  QGraphicsScene mapScene_;
  ProbeOptimizer *probeOptimizer_;
  std::vector<GraphicsItemPtr> siteWidgets_;
  std::vector<GraphicsItemPtr> linkGraphics_;
  std::vector<GraphicsItemPtr> comboGraphics_;

private Q_SLOTS:
  void calculateSiteWidgets();
  void calculateLinks();
};

#endif // MIRAMAP_H
