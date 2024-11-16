//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_SITE_LIST_H
#define XENOPROBES_SITE_LIST_H

#include <vector>
#include "site.h"
#include "ore.h"

class SiteList {
public:
    void addNeighborToSite(size_t site_index, size_t neighbor_index);
    void loadSites();
    long findIndexForSiteName(int name) const;
    Site getSite(size_t index) const;
    long findIndexForOreName(const std::string& name) const;
    Ore getOreByIndex(size_t index) const;
    void clear();

    size_t size() const;
    size_t getOreCount() const;

private:
    std::vector<Site> sites_;
    std::vector<Ore> ores_;
};


#endif //XENOPROBES_SITE_LIST_H
