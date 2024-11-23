/**
 * @file SiteListLoader.cpp
 *
 * @author Dan Keenan
 * @date 11/23/24
 * @copyright GNU GPLv3
 */

#include "SiteListLoader.h"
#include <QFile>
#include <QTextStream>

FnSite::IdList SiteListLoader::readSiteList(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw std::runtime_error("Failed to open file.");
    }
    QTextStream in(&file);
    FnSite::IdList ids;
    QString line;
    bool ok;
    while (in.readLineInto(&line))
    {
        line = line.trimmed();
        if (line.isEmpty() || line.startsWith('#'))
        {
            // Comment or blank line.
            continue;
        }
        const auto idStr = line.first(line.indexOf(','));
        const auto id = idStr.toUInt(&ok, 10);
        // Some sanity checking on the file.
        if (!ok)
        {
            throw std::runtime_error("Failed to parse id.");
        }
        if (!FnSite::kAllSites.contains(id))
        {
            throw std::runtime_error("Site not found.");
        }

        ids.insert(id);
    }

    return ids;
}

void SiteListLoader::writeSiteList(const FnSite::IdList& ids, const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
        throw std::runtime_error("Failed to open file.");
    }

    // Write header.
    QTextStream out(&file);
    out << "# Format:\n"
        << "#   SiteName,MiningGrade,RevenueGrade,CombatGrade,SightseeingSpots,RareOre,RareOre,RareOre,...\n"
        << "#\n"
        << "# Everything after a # will be ignored.\n";
    // Write all sites, commenting out those that are not in ids.
    // Sort the sites by id.
    std::vector allSites(FnSite::kAllSites.begin(), FnSite::kAllSites.end());
    std::sort(allSites.begin(), allSites.end(), [](const FnSite& a, const FnSite& b)
    {
        return a.id < b.id;
    });

    FnSite::Id lastId = 0;
    for (const auto& site : allSites)
    {
        if (site.id / 100 != lastId / 100)
        {
            out << '\n';
        }
        if (!ids.contains(site.id))
        {
            out << '#';
        }
        out << site.id << ',' << FnSite::gradeToChar(site.miningGrade) << ',' << FnSite::gradeToChar(site.revenueGrade)
            << ',' << FnSite::gradeToChar(site.combatGrade) << ',' << site.sightseeingSpots;
        for (const auto& ore : site.ore)
        {
            out << ',' << ore;
        }
        out << '\n';
        lastId = site.id;
    }
    out.flush();
}
