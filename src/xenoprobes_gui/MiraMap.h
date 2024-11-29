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

#include "FnSite.h"
#include "FnSiteWidget.h"
#include "QGraphicsItemDeleter.h"

class MiraMap : public QGraphicsView {
  Q_OBJECT

public:
  using SiteProbeMap = QHash<FnSite::Id, DataProbe::Id>;

  [[nodiscard]] static QJsonValue
  siteProbesToJson(const SiteProbeMap &siteProbeMap);
  [[nodiscard]] static SiteProbeMap siteProbesFromJson(const QJsonValue &json);

  explicit MiraMap(QWidget *parent = nullptr);
  [[nodiscard]] const FnSite::IdList &sitesVisited() const {
    return sitesVisited_;
  }
  void setSitesVisited(const FnSite::IdList &sitesVisited);
  [[nodiscard]] const SiteProbeMap &siteProbeMap() const {
    return siteProbeMap_;
  }
  void setSiteProbeMap(const SiteProbeMap &siteProbeMap);
  void setViewMode(const FnSiteWidget::ViewMode viewMode);

protected:
  void wheelEvent(QWheelEvent *event) override;

private:
  static constexpr auto kZSites = 0;
  static constexpr auto kZLinks = -10;
  static constexpr auto kZMap = -100;
  QGraphicsScene mapScene_;
  FnSite::IdList sitesVisited_;
  SiteProbeMap siteProbeMap_;
  std::vector<std::unique_ptr<QGraphicsItem, QGraphicsItemDeleter>>
      siteWidgets_;
  std::vector<std::unique_ptr<QGraphicsItem, QGraphicsItemDeleter>>
      linkGraphics_;

private Q_SLOTS:
  void calculateSiteWidgets();
  void calculateLinks();
};

#endif // MIRAMAP_H
