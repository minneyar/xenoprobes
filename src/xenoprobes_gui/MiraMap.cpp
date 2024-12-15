/**
 * @file MiraMap.cpp
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#include "MiraMap.h"
#include "FnSiteWidget.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QJsonArray>
#include <ranges>

MiraMap::MiraMap(QWidget *parent) : QGraphicsView(parent) {
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  setDragMode(ScrollHandDrag);
  setMouseTracking(true);

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
  for (const auto &node : Site::ALL | std::views::values) {
    auto siteWidget = new FnSiteWidget(&node);
    siteWidget->setVisited(sitesVisited_.contains(node.name));
    connect(siteWidget, &FnSiteWidget::visitedChanged,
            [this, &node](const bool visited) {
              if (visited) {
                sitesVisited_.insert(node.name);
              } else {
                sitesVisited_.erase(node.name);
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
    siteButton->setX(node.position.first - (siteWidget->width() / 2.0));
    siteButton->setY(node.position.second - (siteWidget->height() / 2.0));
    siteButton->setZValue(kZSites);
  }
  calculateLinks();
}

void MiraMap::setSitesVisited(
    const std::unordered_set<Site::Id> &sitesVisited) {
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
  for (const auto &[siteId, probeId] : siteProbeMap) {
    json.append(QJsonArray(
        {static_cast<int>(siteId), QString::fromStdString(probeId)}));
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
    if (!siteId.isDouble()) {
      throw std::runtime_error("Bad site id.");
    }
    Site::Ptr site;
    try {
      site = Site::fromName(siteId.toInt());
    } catch (const std::out_of_range &) {
      throw std::runtime_error("Bad site id.");
    }
    const auto probeId = siteProbeInfo[1];
    if (!probeId.isString()) {
      throw std::runtime_error("Bad probe id.");
    }
    Probe::Ptr probe;
    try {
      probe = Probe::fromString(probeId.toString().toStdString());
    } catch (const std::out_of_range &) {
      throw std::runtime_error("Bad probe id.");
    }
    siteProbeMap.emplace(site->name, probe->id);
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
    fnSiteWidget->setVisited(
        sitesVisited_.contains(fnSiteWidget->site()->name));
    const auto dataProbe = siteProbeMap_.find(fnSiteWidget->site()->name);
    if (dataProbe == siteProbeMap_.end()) {
      fnSiteWidget->setDataProbe(nullptr);
    } else {
      fnSiteWidget->setDataProbe(dataProbe->second);
    }
  }

  calculateLinks();
}

void MiraMap::calculateLinks() {
  QPen pen(Qt::SolidLine);
  pen.setWidth(4);
  pen.setColor(Qt::white);
  linkGraphics_.clear();
  // Need to track which lines have already been drawn as both sides of the link
  // are stored.
  std::vector<std::unordered_set<Site::Id>> drawnLinks;
  for (const auto &site : Site::ALL | std::views::values) {
    for (const auto neighbor : site.getNeighbors()) {
      const std::unordered_set linkPair{site.name, neighbor->name};
      if (std::find(drawnLinks.cbegin(), drawnLinks.cend(), linkPair) !=
          drawnLinks.cend()) {
        // Already drawn line.
        continue;
      }
      const auto [x1, y1] = site.position;
      const auto [x2, y2] = neighbor->position;
      auto &linkItem = linkGraphics_.emplace_back(
          mapScene_.addLine(QLine(x1, y1, x2, y2), pen));
      linkItem->setZValue(kZLinks);
      drawnLinks.emplace_back(std::move(linkPair));
    }
  }
}
