/**
 * @file FnSite.h
 *
 * @author Dan Keenan
 * @date 11/17/24
 * @copyright GNU GPLv3
 */

#ifndef FNSITE_H
#define FNSITE_H

#include <QStringList>
#include <QPoint>
#include <QHash>
#include <unordered_set>
#include <vector>

struct FnSite
{
    using Id = unsigned int;
    using IdList = std::unordered_set<Id>;
    enum class Grade
    {
        F,
        E,
        D,
        C,
        B,
        A,
        S
    };

    static const QHash<Id, FnSite> kAllSites;
    static const std::vector<std::array<Id, 2>> kAllLinks;

    unsigned int id;
    QPoint position;
    Grade miningGrade;
    Grade revenueGrade;
    Grade combatGrade;
    unsigned int sightseeingSpots;
    QStringList ore;
};


#endif //FNSITE_H
