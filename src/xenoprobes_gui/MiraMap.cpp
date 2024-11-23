/**
 * @file MiraMap.cpp
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#include "MiraMap.h"
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QGraphicsProxyWidget>

#include "FnSite.h"

MiraMap::MiraMap(FnSite::IdList* sitesVisited, QWidget* parent) : QGraphicsView(parent), sitesVisited_(sitesVisited)
{
    setDragMode(ScrollHandDrag);

    // Add map tiles.
    const auto tileSize = 256; // 256x256 pixels
    const auto mapSize = 16; // 16x16 tile grid
    mapScene_.setSceneRect(0, 0, tileSize * mapSize, tileSize * mapSize);
    for (auto tileX = 0; tileX < mapSize; ++tileX)
    {
        for (auto tileY = 0; tileY < mapSize; ++tileY)
        {
            QPixmap tilePixmap;
            // Not all tiles are present, only those with map images.
            if (tilePixmap.load(QString(":/map_tiles/tile_%1_%2.png").arg(tileX).arg(tileY)))
            {
                auto* scenePixmap = mapScene_.addPixmap(tilePixmap);
                scenePixmap->setOffset(tileX * tileSize, tileY * tileSize);
                scenePixmap->setZValue(kZMap);
            }
        }
    }

    // Add FN Sites.
    const auto siteSize = 64;
    // Site data stores the center point, so we need to half it to get the corners for drawing.
    const auto cornerOffset = siteSize / 2;
    for (const auto& node : FnSite::kAllSites)
    {
        auto siteWidget = new QPushButton(QString::number(node.id));
        siteWidget->setFixedSize(siteSize, siteSize);
        auto siteButton = mapScene_.addWidget(siteWidget);
        siteButton->setX(node.position.x() - cornerOffset);
        siteButton->setY(node.position.y() - cornerOffset);
        siteButton->setZValue(kZSites);
    }

    calculateLinks();

    setScene(&mapScene_);
}

void MiraMap::wheelEvent(QWheelEvent* event)
{
    if (qApp->queryKeyboardModifiers() == Qt::ControlModifier)
    {
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

void MiraMap::calculateLinks()
{
    QPen pen(Qt::SolidLine);
    pen.setWidth(4);
    pen.setColor(Qt::white);
    linkGraphics_.clear();
    for (const auto& link : FnSite::kAllLinks)
    {
        if (linkVisited(link))
        {
            const auto linkA = FnSite::kAllSites.value(link[0]);
            const auto linkB = FnSite::kAllSites.value(link[1]);
            auto& linkItem = linkGraphics_.emplace_back(
                mapScene_.addLine(QLine(linkA.position, linkB.position), pen)
            );
            linkItem->setZValue(kZLinks);
        }
    }
}
