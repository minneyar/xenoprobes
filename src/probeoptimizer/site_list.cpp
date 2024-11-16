//
// Created by preed on 1/6/16.
//

#include <iosfwd>
#include <map>
#include <iostream>
#include <fstream>
#include "probeoptimizer/site_list.h"
#include "probeoptimizer/probe_optimizer.h"

void SiteList::addNeighborToSite(size_t site_index, size_t neighbor_index) {
    sites_[site_index].addNeighbor(neighbor_index);
    sites_[neighbor_index].addNeighbor(site_index);
}

void SiteList::loadSites() {
    sites_.clear();

    std::map<std::string, int> prodGrade = {
            {"A", 500},
            {"B", 350},
            {"C", 250}
    };
    std::map<std::string, int> revGrade = {
            {"S", 850},
            {"A", 750},
            {"B", 650},
            {"C", 550},
            {"D", 450},
            {"E", 300},
            {"F", 200}
    };


    try {
        auto data = ProbeOptimizer::loadCSV("sites.csv");
        for (const auto& record : data) {
            Site site(std::stoi(record[0]));
            site.setProduction(prodGrade.at(record[1]));
            site.setRevenue(revGrade.at(record[2]));
            site.setSightseeing(std::stoi(record[4]));
            if (site.getSightseeing() < 0 || site.getSightseeing() > 2)
                std::cerr << "\n**WARNING**: site " << record[0]
                << " looks wrong: sightseeing spots should be in [0..2], but is "
                << record[4] << std::endl << std::endl;
            for (size_t i=5; i<record.size(); ++i) {
                const auto& oreName = record[i];
                int oreIdx = findIndexForOreName(oreName);
                if (oreIdx == -1) {
                    oreIdx = int(ores_.size());
                    ores_.push_back({oreIdx, oreName, 0});
                }
                site.addOre(oreIdx);
            }
            sites_.push_back(std::move(site));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error while loading 'sites.csv': " << e.what() << std::endl;
        throw;
    }

    int numConnections = 0;

    {
        std::ifstream file{"mira.dot"};
        std::string line;
        while (getline(file, line)) {
            int u, v;
            if (std::sscanf(line.c_str(), "%d -- %d", &u, &v) == 2) {
                int uidx = findIndexForSiteName(u);
                int vidx = findIndexForSiteName(v);
                if (uidx != -1 && vidx != -1) {
                    addNeighborToSite(uidx, vidx);
                    ++numConnections;
                }
            }
        }
    }

    std::clog << "Loaded " << sites_.size() << " FN sites with " <<
            numConnections << " connections." << std::endl;

}

long SiteList::findIndexForSiteName(int name) const {
    for (int i=0; i< sites_.size(); ++i) {
        if (sites_[i].getName() == name) {
            return i;
        }
    }
    return -1;
}

Site SiteList::getSite(size_t index) const {
    return sites_[index];
}

long SiteList::findIndexForOreName(const std::string& name) const {
    for (int i = 0; i < ores_.size(); i++) {
        if (ores_[i].getName() == name) {
            return i;
        }
    }
    return -1;
}

void SiteList::clear() {
    sites_.clear();
}

size_t SiteList::size() const {
    return sites_.size();
}

Ore SiteList::getOreByIndex(size_t index) const {
    return ores_[index];
}

size_t SiteList::getOreCount() const {
    return ores_.size();
}
