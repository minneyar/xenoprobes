//
// Created by preed on 1/6/16.
//

#include "probeoptimizer/site_list.h"
#include "probeoptimizer/csv.h"
#include <iosfwd>
#include <spdlog/spdlog.h>

std::vector<Site::Ptr> loadSiteList(const std::string &filename) {
  try {
    auto data = loadCSV(filename);
    return loadSiteList(data);
  } catch (std::exception &e) {
    spdlog::error("Error while loading {}: {}", filename, e.what());
    throw;
  }
}

std::vector<Site::Ptr>
loadSiteList(const std::vector<std::vector<CsvRecordVal>> &records) {
  try {
    std::unordered_set<Site::Id> idList;

    for (const auto &record : records) {
      // Ignore all other columns in the list as those values are compiled in.
      const auto siteId = csvRecordValToInt(record[0]);
      idList.insert(siteId);
    }

    return loadSiteList(idList);
  } catch (std::exception &e) {
    spdlog::error("Bad site data format: {}", e.what());
    throw;
  }
}

std::vector<Site::Ptr>
loadSiteList(const std::unordered_set<Site::Id> &idList) {
  std::vector<Site::Ptr> sites;
  for (const auto id : idList) {
    try {
      sites.push_back(Site::fromName(id));
    } catch (std::out_of_range &) {
      spdlog::error("Could not find ID {}", idList.size());
      throw;
    }
  }

  int numConnections = 0;
  for (const auto &site : sites) {
    numConnections += site->getNeighbors().size();
  }
  // Counted both edges of the connection.
  numConnections /= 2;
  spdlog::info("Loaded {} FN sites with {} connections.", sites.size(),
               numConnections);
  return sites;
}
