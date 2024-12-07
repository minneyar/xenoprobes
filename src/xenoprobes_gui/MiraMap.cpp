/**
 * @file MiraMap.cpp
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#include "MiraMap.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QJsonArray>
#include <probeoptimizer/site_links.h>

#include "FnSite.h"
#include "FnSiteWidget.h"

MiraMap::MiraMap(QWidget *parent) : QGraphicsView(parent) {
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  setDragMode(ScrollHandDrag);

  // Add map tiles.
  const auto tileSize = 256; // 256x256 pixels
  const auto mapSize = 16;   // 16x16 tile grid
  mapScene_.setSceneRect(0, 0, tileSize * mapSize, tileSize * mapSize);
  for (auto tileX = 0; tileX < mapSize; ++tileX) {
    for (auto tileY = 0; tileY < mapSize; ++tileY) {
      QPixmap tilePixmap;
      // Not all tiles are present, only those with map images.
      if (tilePixmap.load(
              QString(":/map_tiles/tile_%1_%2.png").arg(tileX).arg(tileY))) {
        auto *scenePixmap = mapScene_.addPixmap(tilePixmap);
        scenePixmap->setOffset(tileX * tileSize, tileY * tileSize);
        scenePixmap->setZValue(kZMap);
      }
    }
  }
  setScene(&mapScene_);

  // Site widgets.
  for (const auto &node : FnSite::kAllSites) {
    auto siteWidget = new FnSiteWidget(node);
    siteWidget->setVisited(sitesVisited_.contains(node.id));
    connect(siteWidget, &FnSiteWidget::visitedChanged,
            [this, &node](const bool visited) {
              if (visited) {
                sitesVisited_.insert(node.id);
              } else {
                sitesVisited_.erase(node.id);
              }
              calculateLinks();
            });
    connect(siteWidget, &FnSiteWidget::visitedChanged, this,
            &MiraMap::sitesVisitedChanged);
    connect(siteWidget, &FnSiteWidget::dataProbeChanged, this,
            &MiraMap::siteProbeMapChanged);
    auto &siteButton =
        siteWidgets_.emplace_back(mapScene_.addWidget(siteWidget));
    // Site data stores the center point, so we need to half it to get the
    // corners for drawing.
    siteButton->setX(node.position.x() - (siteWidget->width() / 2.0));
    siteButton->setY(node.position.y() - (siteWidget->height() / 2.0));
    siteButton->setZValue(kZSites);
  }
  calculateLinks();
}

void MiraMap::setSitesVisited(const FnSite::IdList &sitesVisited) {
  sitesVisited_ = sitesVisited;
  calculateSiteWidgets();
}

void MiraMap::setSiteProbeMap(const SiteProbeMap &siteProbeMap) {
  siteProbeMap_ = siteProbeMap;
  calculateSiteWidgets();
}

void MiraMap::setViewMode(const FnSiteWidget::ViewMode viewMode) {
  for (auto &widget : siteWidgets_) {
    auto *fnSiteWidget = dynamic_cast<FnSiteWidget *>(
        dynamic_cast<QGraphicsProxyWidget *>(widget.get())->widget());
    fnSiteWidget->setViewMode(viewMode);
  }
}

QJsonValue MiraMap::siteProbesToJson(const SiteProbeMap &siteProbeMap) {
  // Do this as an array of 2-tuples so we don't have to worry about int parsing
  // when reading.
  QJsonArray json;
  for (const auto &[siteId, probeId] : siteProbeMap.asKeyValueRange()) {
    json.append(QJsonArray({static_cast<int>(siteId), probeId}));
  }

  return json;
}
MiraMap::SiteProbeMap MiraMap::siteProbesFromJson(const QJsonValue &json) {
  if (!json.isArray()) {
    throw std::runtime_error("Bad site probes map format.");
  }
  SiteProbeMap siteProbeMap;
  siteProbeMap.reserve(json.toArray().size());
  for (const auto &siteProbe : json.toArray()) {
    if (!siteProbe.isArray() || siteProbe.toArray().size() != 2) {
      throw std::runtime_error("Bad site probe format.");
    }
    const auto siteProbeInfo = siteProbe.toArray();
    const auto siteId = siteProbeInfo[0];
    if (!siteId.isDouble() || !FnSite::kAllSites.contains(siteId.toInt())) {
      throw std::runtime_error("Bad site id.");
    }
    const auto probeId = siteProbeInfo[1];
    if (!probeId.isString() ||
        !DataProbe::kAllProbes.contains(probeId.toString())) {
      throw std::runtime_error("Bad probe id.");
    }
    siteProbeMap.emplace(siteId.toInt(), probeId.toString());
  }
  return siteProbeMap;
}

void MiraMap::wheelEvent(QWheelEvent *event) {
  if (qApp->queryKeyboardModifiers() == Qt::ControlModifier) {
    // Ctrl+Wheel to zoom.
    event->accept();
    const auto degrees = event->angleDelta() / 8;
    const auto scaleChange = degrees.y() / 100.0;

    // Scaling is relative to current scale, not absolute.
    const auto newScale = scaleChange + 1.0;
    scale(newScale, newScale);
    return;
  }
  QGraphicsView::wheelEvent(event);
}

void MiraMap::calculateSiteWidgets() {
  for (auto &widget : siteWidgets_) {
    auto *fnSiteWidget = dynamic_cast<FnSiteWidget *>(
        dynamic_cast<QGraphicsProxyWidget *>(widget.get())->widget());
    fnSiteWidget->setVisited(sitesVisited_.contains(fnSiteWidget->site().id));
    const auto dataProbe = siteProbeMap_.find(fnSiteWidget->site().id);
    if (dataProbe == siteProbeMap_.end()) {
      fnSiteWidget->setDataProbe(nullptr);
    } else {
      fnSiteWidget->setDataProbe(dataProbe.value());
    }
  }

  calculateLinks();
}

void MiraMap::calculateLinks() {
  QPen pen(Qt::SolidLine);
  pen.setWidth(4);
  pen.setColor(Qt::white);
  linkGraphics_.clear();
  for (const auto &link : kAllSiteLinks) {
    const auto linkVisited =
        sitesVisited_.contains(link[0]) && sitesVisited_.contains(link[1]);
    if (linkVisited) {
      const auto linkA = FnSite::kAllSites.value(link[0]);
      const auto linkB = FnSite::kAllSites.value(link[1]);
      auto &linkItem = linkGraphics_.emplace_back(
          mapScene_.addLine(QLine(linkA.position, linkB.position), pen));
      linkItem->setZValue(kZLinks);
    }
  }
}
