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
#include "QGraphicsItemDeleter.h"


class MiraMap : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MiraMap(FnSite::IdList* sitesVisited, QWidget* parent = nullptr);
    void setSitesVisited(FnSite::IdList* sitesVisited);

protected:
    void wheelEvent(QWheelEvent* event) override;

private:
    static constexpr auto kZSites = 0;
    static constexpr auto kZLinks = -10;
    static constexpr auto kZMap = -100;
    QGraphicsScene mapScene_;
    FnSite::IdList* sitesVisited_;
    std::vector<std::unique_ptr<QGraphicsItem, QGraphicsItemDeleter>> linkGraphics_;

    bool linkVisited(const decltype(FnSite::kAllLinks)::value_type& link) const
    {
        return sitesVisited_->contains(link[0]) && sitesVisited_->contains(link[1]);
    }

private Q_SLOTS:
    void calculateLinks();
};


#endif //MIRAMAP_H
