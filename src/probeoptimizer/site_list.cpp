//
// Created by preed on 1/6/16.
//

#include "probeoptimizer/site_list.h"
#include "probeoptimizer/csv.h"
#include "probeoptimizer/probe_optimizer.h"
#include "probeoptimizer/site_links.h"
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <map>
#include <spdlog/spdlog.h>

const std::map<std::string, int> prodGrade{{"A", 500}, {"B", 350}, {"C", 250}};
const std::map<std::string, int> revGrade{{"S", 850}, {"A", 750}, {"B", 650},
                                          {"C", 550}, {"D", 450}, {"E", 300},
                                          {"F", 200}};

void SiteList::addNeighborToSite(size_t site_index, size_t neighbor_index) {
  sites_[site_index].addNeighbor(neighbor_index);
  sites_[neighbor_index].addNeighbor(site_index);
}

void SiteList::loadSites(const std::string &filename) {
  try {
    auto data = loadCSV(filename);
    loadSites(data);
  } catch (std::exception &e) {
    spdlog::error("Error while loading {}: {}", filename, e.what());
    throw;
  }
}

void SiteList::loadSites(
    const std::vector<std::vector<CsvRecordVal>> &records) {
  sites_.clear();

  try {
    for (const auto &record : records) {
      Site site(csvRecordValToInt(record[0]));
      site.setProduction(prodGrade.at(std::get<std::string>(record[1])));
      site.setRevenue(revGrade.at(std::get<std::string>(record[2])));
      site.setSightseeing(csvRecordValToInt(record[4]));
      if (site.getSightseeing() < 0 || site.getSightseeing() > 2)
        spdlog::warn("site {} looks wrong: sightseeing spots should be in "
                     "[0..2], but is {}.",
                     site.getSightseeing(), site.getSightseeing());
      for (size_t i = 5; i < record.size(); ++i) {
        const auto &oreName = std::get<std::string>(record[i]);
        int oreIdx = findIndexForOreName(oreName);
        if (oreIdx == -1) {
          oreIdx = int(ores_.size());
          ores_.push_back({oreIdx, oreName, 0});
        }
        site.addOre(oreIdx);
      }
      sites_.push_back(std::move(site));
    }
  } catch (std::exception &e) {
    spdlog::error("Bad site data format: {}", e.what());
    throw;
  }
  addConnections();

  int numConnections = 0;
  for (const auto &site : sites_) {
    numConnections += site.getNeighbors().size();
  }
  // Counted both edges of the connection.
  numConnections /= 2;
  spdlog::info("Loaded {} FN sites with {} connections.", sites_.size(),
               numConnections);
}

void SiteList::addConnections() {
  for (const auto [u, v] : kAllSiteLinks) {
    const auto uidx = findIndexForSiteName(u);
    const auto vidx = findIndexForSiteName(v);
    if (uidx != -1 && vidx != -1) {
      addNeighborToSite(uidx, vidx);
    }
  }
}

long SiteList::findIndexForSiteName(int name) const {
  for (int i = 0; i < sites_.size(); ++i) {
    if (sites_[i].getName() == name) {
      return i;
    }
  }
  return -1;
}

Site SiteList::getSite(size_t index) const { return sites_[index]; }

long SiteList::findIndexForOreName(const std::string &name) const {
  for (int i = 0; i < ores_.size(); i++) {
    if (ores_[i].getName() == name) {
      return i;
    }
  }
  return -1;
}

void SiteList::clear() { sites_.clear(); }

size_t SiteList::size() const { return sites_.size(); }

Ore SiteList::getOreByIndex(size_t index) const { return ores_[index]; }

size_t SiteList::getOreCount() const { return ores_.size(); }
