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
#include <QMenu>
#include <boost/container_hash/hash.hpp>
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
  for (const auto site : Site::ALL | std::views::values) {
    auto siteWidget = new FnSiteWidget(site);
    connect(siteWidget, &FnSiteWidget::visitedChanged,
            [this, site](const bool visited) {
              if (visited) {
                probeOptimizer_->addSite(site);
              } else {
                probeOptimizer_->removeSite(site);
              }
              calculateSiteWidgets();
              Q_EMIT(sitesVisitedChanged());
            });
    connect(siteWidget, &FnSiteWidget::dataProbeChanged,
            [this, site](const Probe::Ptr probe) {
              probeOptimizer_->setProbeAt(site, probe);
              calculateLinks();
              Q_EMIT(siteProbeMapChanged());
            });
    auto &siteButton =
        siteWidgets_.emplace_back(mapScene_.addWidget(siteWidget));
    // Site data stores the center point, so we need to half it to get the
    // corners for drawing.
    siteButton->setX(site->position.first - (siteWidget->width() / 2.0));
    siteButton->setY(site->position.second - (siteWidget->height() / 2.0));
    siteButton->setZValue(kZSites);
  }
  calculateSiteWidgets();
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

void MiraMap::contextMenuEvent(QContextMenuEvent *event) {
  // If the widget is responsible for popping its own menu, it is scaled with
  // the rest of the view which makes it hard to read. Popping it here (in the
  // scene) does not scale the menu.
  const auto item = itemAt(event->pos());
  const auto widget = qgraphicsitem_cast<QGraphicsProxyWidget *>(item);
  if (widget == nullptr) {
    return;
  }

  // Add the widget's actions to this menu.
  QMenu menu;
  for (const auto action : widget->widget()->actions()) {
    menu.addAction(action);
  }
  menu.exec(event->globalPos());

  event->accept();
}

void MiraMap::probeContextMenuEvent(QContextMenuEvent *event) {}

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

static const auto noComboLinkColor = QColorConstants::Svg::cyan;
static const auto withComboLinkColor = QColorConstants::Svg::deeppink;
static const auto comboCircleColor = QColorConstants::Svg::red;

void MiraMap::calculateLinks() {
  QPen pen(Qt::SolidLine);
  pen.setWidth(4);
  linkGraphics_.clear();
  comboGraphics_.clear();
  // Need to track which lines have already been drawn as both sides of the link
  // are stored.
  std::unordered_set<std::set<Site::Id>, boost::hash<std::set<Site::Id>>>
      drawnLinks;
  for (const auto site : Site::ALL | std::views::values) {
    if (!probeOptimizer_->getSites().contains(site)) {
      // Do not draw links between sites not visited.
      continue;
    }
    const auto comboBonus =
        probeOptimizer_->solution().getSetup().getComboBonus(
            ProbeOptimizer::getIndexForSiteId(site->name));
    for (const auto neighbor : site->getNeighbors()) {
      // Don't draw combo lines more than once.
      const std::set linkPair{site->name, neighbor->name};
      if (drawnLinks.contains(linkPair)) {
        // Already drawn line.
        continue;
      }

      bool comboLink = false;
      if (comboBonus > 1) {
        // This site participates in a combo, determine if this link is part of
        // it.
        const auto siteProbe =
            probeOptimizer_->solution().getSetup().getProbeAt(
                ProbeOptimizer::getIndexForSiteId(site->name));
        const auto neighborProbe =
            probeOptimizer_->solution().getSetup().getProbeAt(
                ProbeOptimizer::getIndexForSiteId(neighbor->name));
        if (siteProbe == neighborProbe) {
          comboLink = true;
        }
      }
      if (comboLink) {
        pen.setColor(withComboLinkColor);
      } else {
        pen.setColor(noComboLinkColor);
      }

      const auto [x1, y1] = site->position;
      const auto [x2, y2] = neighbor->position;
      auto &linkItem = linkGraphics_.emplace_back(
          mapScene_.addLine(QLine(x1, y1, x2, y2), pen));
      linkItem->setZValue(kZLinks);
      drawnLinks.insert(linkPair);
    }

    // Draw circles on sites that are part of a combo.
    if (comboBonus > 1) {
      pen.setColor(comboCircleColor);
      auto [x, y] = site->position;
      const auto w = 96;
      const auto h = w;
      x -= w / 2;
      y -= h / 2;
      auto &comboItem = comboGraphics_.emplace_back(
          mapScene_.addEllipse(QRect(x, y, w, h), pen));
      comboItem->setZValue(kZCombo);
    }
  }
}
