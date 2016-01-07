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
    void addNeighborToSite(int site_index, int neighbor_index);
    void loadSites();
    int findIndexForSiteName(int name) const;
    Site getSite(int index) const;
    int findIndexForOreName(const std::string& name) const;
    Ore getOreByIndex(int index) const;
    void clear();

    unsigned long size() const;
    unsigned long getOreCount() const;

private:
    std::vector<Site> sites_;
    std::vector<Ore> ores_;
};


#endif //XENOPROBES_SITE_LIST_H
