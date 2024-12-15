//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_SITE_LIST_H
#define XENOPROBES_SITE_LIST_H

#include "csv.h"

#include "ore.h"
#include "site.h"

#include <unordered_set>
#include <vector>

std::vector<Site::Ptr> loadSiteList(const std::string &filename);
std::vector<Site::Ptr>
loadSiteList(const std::vector<std::vector<CsvRecordVal>> &records);
std::vector<Site::Ptr> loadSiteList(const std::unordered_set<Site::Id> &idList);

#endif // XENOPROBES_SITE_LIST_H
