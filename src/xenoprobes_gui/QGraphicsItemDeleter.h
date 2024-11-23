/**
 * @file QGraphicsItemDeleter.h
 *
 * @author Dan Keenan
 * @date 11/23/24
 * @copyright GNU GPLv3
 */

#ifndef QGRAPHICSITEMDELETER_H
#define QGRAPHICSITEMDELETER_H

#include <QGraphicsItem>
#include <QGraphicsScene>

/**
 * Deleter for QGraphicsItem.
 *
 * Removes item from scene (if applicable) before deleting.
 */
struct QGraphicsItemDeleter
{
    void operator()(QGraphicsItem* item) const
    {
        if (auto* scene = item->scene())
        {
            scene->removeItem(item);
        }
        delete item;
    }
};


#endif //QGRAPHICSITEMDELETER_H
