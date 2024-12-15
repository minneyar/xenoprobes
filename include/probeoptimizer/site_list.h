//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_SITE_LIST_H
#define XENOPROBES_SITE_LIST_H

#include "csv.h"

#include "ore.h"
#include "site.h"
#include <vector>

std::vector<Site::Ptr> loadSiteList(const std::string &filename);
std::vector<Site::Ptr>
loadSiteList(const std::vector<std::vector<CsvRecordVal>> &records);

#endif // XENOPROBES_SITE_LIST_H
