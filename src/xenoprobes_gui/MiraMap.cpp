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
#include <QGuiApplication>
#include <QJsonArray>
#include <ranges>

MiraMap::MiraMap(ProbeOptimizer *probeOptimizer, QWidget *parent)
    : QGraphicsView(parent), probeOptimizer_(probeOptimizer) {
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
  for (const auto &siteId : Site::ALL | std::views::keys) {
    const auto site = Site::fromName(siteId);
    auto siteWidget = new FnSiteWidget(site);
    siteWidget->setVisited(probeOptimizer_->getSites().contains(site));
    connect(siteWidget, &FnSiteWidget::visitedChanged,
            [this, site](const bool visited) {
              if (visited) {
                probeOptimizer_->addSite(site);
              } else {
                probeOptimizer_->removeSite(site);
              }
              calculateSiteWidgets();
            });
    connect(siteWidget, &FnSiteWidget::visitedChanged, this,
            &MiraMap::sitesVisitedChanged);
    connect(siteWidget, &FnSiteWidget::dataProbeChanged, this,
            &MiraMap::siteProbeMapChanged);
    auto &siteButton =
        siteWidgets_.emplace_back(mapScene_.addWidget(siteWidget));
    // Site data stores the center point, so we need to half it to get the
    // corners for drawing.
    siteButton->setX(site->position.first - (siteWidget->width() / 2.0));
    siteButton->setY(site->position.second - (siteWidget->height() / 2.0));
    siteButton->setZValue(kZSites);
  }
  calculateLinks();
}

void MiraMap::setProbeOptimizer(ProbeOptimizer *probeOptimizer) {
  probeOptimizer_ = probeOptimizer;
  calculateSiteWidgets();
}

void MiraMap::setViewMode(const FnSiteWidget::ViewMode viewMode) {
  for (auto &widget : siteWidgets_) {
    auto *fnSiteWidget = dynamic_cast<FnSiteWidget *>(
        dynamic_cast<QGraphicsProxyWidget *>(widget.get())->widget());
    fnSiteWidget->setViewMode(viewMode);
  }
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
    const auto site = fnSiteWidget->site();
    const auto visited = probeOptimizer_->getSites().contains(site);
    fnSiteWidget->setVisited(visited);
    if (visited) {
      const auto dataProbe = probeOptimizer_->solution().getSetup().getProbeAt(
          ProbeOptimizer::getIndexForSiteId(site->name));
      if (dataProbe == nullptr) {
        fnSiteWidget->setDataProbe(nullptr);
      } else {
        fnSiteWidget->setDataProbe(dataProbe);
      }
    } else {
      fnSiteWidget->setDataProbe(nullptr);
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
  for (const auto &siteId : Site::ALL | std::views::keys) {
    const auto site = Site::fromName(siteId);
    if (!probeOptimizer_->getSites().contains(site)) {
      // Do not draw links between sites not visited.
      continue;
    }
    for (const auto neighbor : site->getNeighbors()) {
      const std::unordered_set linkPair{site->name, neighbor->name};
      if (std::find(drawnLinks.cbegin(), drawnLinks.cend(), linkPair) !=
          drawnLinks.cend()) {
        // Already drawn line.
        continue;
      }
      const auto [x1, y1] = site->position;
      const auto [x2, y2] = neighbor->position;
      auto &linkItem = linkGraphics_.emplace_back(
          mapScene_.addLine(QLine(x1, y1, x2, y2), pen));
      linkItem->setZValue(kZLinks);
      drawnLinks.emplace_back(std::move(linkPair));
    }
  }
}
