//
// Created by preed on 1/6/16.
//

#include "probeoptimizer/site.h"
#include <spdlog/spdlog.h>

Site::Grade Site::gradeFromChar(char grade) {
  switch (grade) {
  case 'A':
    return Site::Grade::A;
  case 'B':
    return Site::Grade::B;
  case 'C':
    return Site::Grade::C;
  case 'D':
    return Site::Grade::D;
  case 'E':
    return Site::Grade::E;
  case 'F':
    return Site::Grade::F;
  case 'S':
    return Site::Grade::S;
  }
  throw std::invalid_argument("Invalid grade");
}

int Site::getName() const { return name_; }

Site::Point Site::getPosition() const { return position_; }

void Site::setPosition(const Point &position) { position_ = position; }

Site::Grade Site::getProduction() const { return production_; }

int Site::getProductionVal() const {
  switch (production_) {
  case Grade::A:
    return 500;
  case Grade::B:
    return 350;
  case Grade::C:
    return 250;
  }
  throw std::invalid_argument("Invalid production grade");
}

void Site::setProduction(Grade grade) { production_ = grade; }

Site::Grade Site::getRevenue() const { return revenue_; }

int Site::getRevenueVal() const {
  switch (revenue_) {
  case Grade::S:
    return 850;
  case Grade::A:
    return 750;
  case Grade::B:
    return 650;
  case Grade::C:
    return 550;
  case Grade::D:
    return 450;
  case Grade::E:
    return 300;
  case Grade::F:
    return 200;
  }
  throw std::invalid_argument("Invalid revenue grade");
}

void Site::setRevenue(Grade grade) { revenue_ = grade; }

Site::Grade Site::getCombat() const { return combat_; }

void Site::setCombat(Grade grade) { combat_ = grade; }

int Site::getSightseeing() const { return sightseeing_; }

void Site::setSightseeing(int i) { sightseeing_ = i; }

void Site::addNeighbor(std::size_t i) { neighbors_.push_back(i); }

std::vector<std::size_t> Site::getNeighbors() const { return neighbors_; }

void Site::addOre(int i) { ores_.push_back(i); }

std::vector<int> Site::getOres() const { return ores_; }
