//
// Created by preed on 1/6/16.
//

#include "probeoptimizer/site.h"

int Site::getName() const {
    return name_;
}

int Site::getProduction() const {
    return production_;
}

void Site::setProduction(int i) {
    production_ = i;
}

int Site::getRevenue() const {
    return revenue_;
}

void Site::setRevenue(int i) {
    revenue_ = i;
}

int Site::getSightseeing() const {
    return sightseeing_;
}

void Site::setSightseeing(int i) {
    sightseeing_ = i;
}

void Site::addNeighbor(std::size_t i) {
    neighbors_.push_back(i);
}

std::vector<std::size_t> Site::getNeighbors() const {
    return neighbors_;
}

void Site::addOre(int i) {
    ores_.push_back(i);
}

std::vector<int> Site::getOres() const {
    return ores_;
}
