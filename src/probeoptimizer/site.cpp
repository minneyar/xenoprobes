//
// Created by preed on 1/6/16.
//

#include "probeoptimizer/site.h"
#include <probeoptimizer/probe_optimizer.h>
#include <spdlog/spdlog.h>

const std::vector<Ore::Ptr> &Site::getOre() const {
  // clang-format off
  static const std::unordered_map<Id, std::vector<Ore::Ptr>> oreMap{
    {101, {}},
    {102, {}},
    {103, {}},
    {104, {}},
    {105, {}},
    {106, {Ore::arcSandOre}},
    {107, {}},
    {108, {Ore::aurorite, Ore::arcSandOre, Ore::foucaultium}},
    {109, {Ore::foucaultium, Ore::dawnstone, Ore::lionboneBort}},
    {110, {Ore::aurorite, Ore::arcSandOre, Ore::whiteCometite, Ore::dawnstone}},
    {111, {Ore::foucaultium}},
    {112, {}},
    {113, {}},
    {114, {}},
    {115, {Ore::arcSandOre, Ore::whiteCometite, Ore::lionboneBort}},
    {116, {}},
    {117, {}},
    {118, {Ore::aurorite, Ore::whiteCometite, Ore::dawnstone}},
    {119, {}},
    {120, {}},
    {121, {}},
    {201, {}},
    {202, {Ore::cimmerianCinnabar, Ore::everfreezeOre}},
    {203, {Ore::cimmerianCinnabar}},
    {204, {}},
    {205, {}},
    {206, {}},
    {207, {Ore::infernium, Ore::whiteCometite, Ore::cimmerianCinnabar, Ore::foucaultium}},
    {208, {Ore::foucaultium}},
    {209, {}},
    {210, {}},
    {211, {}},
    {212, {Ore::aurorite, Ore::enduronLead, Ore::whiteCometite}},
    {213, {}},
    {214, {}},
    {215, {Ore::aurorite, Ore::enduronLead, Ore::everfreezeOre}},
    {216, {}},
    {217, {Ore::aurorite, Ore::infernium, Ore::cimmerianCinnabar}},
    {218, {Ore::aurorite, Ore::enduronLead, Ore::whiteCometite}},
    {219, {Ore::enduronLead, Ore::whiteCometite}},
    {220, {Ore::infernium, Ore::everfreezeOre}},
    {221, {}},
    {222, {}},
    {223, {}},
    {224, {}},
    {225, {}},
    {301, {Ore::infernium, Ore::arcSandOre, Ore::lionboneBort}},
    {302, {}},
    {303, {Ore::aurorite, Ore::whiteCometite}},
    {304, {}},
    {305, {Ore::aurorite, Ore::arcSandOre, Ore::enduronLead}},
    {306, {}},
    {307, {Ore::infernium, Ore::arcSandOre, Ore::enduronLead, Ore::whiteCometite}},
    {308, {Ore::ouroborosCrystal}},
    {309, {Ore::enduronLead, Ore::ouroborosCrystal}},
    {310, {}},
    {311, {}},
    {312, {Ore::infernium, Ore::boiledEggOre, Ore::lionboneBort}},
    {313, {}},
    {314, {}},
    {315, {}},
    {316, {}},
    {317, {}},
    {318, {Ore::boiledEggOre, Ore::whiteCometite, Ore::lionboneBort}},
    {319, {Ore::infernium, Ore::boiledEggOre}},
    {320, {Ore::aurorite, Ore::ouroborosCrystal}},
    {321, {}},
    {322, {}},
    {401, {Ore::parhelionPlatinum, Ore::marineRutile}},
    {402, {}},
    {403, {}},
    {404, {}},
    {405, {Ore::arcSandOre}},
    {406, {}},
    {407, {}},
    {408, {Ore::aurorite, Ore::arcSandOre, Ore::everfreezeOre}},
    {409, {}},
    {410, {}},
    {411, {}},
    {412, {}},
    {413, {}},
    {414, {Ore::parhelionPlatinum, Ore::marineRutile}},
    {415, {}},
    {416, {}},
    {417, {Ore::everfreezeOre, Ore::boiledEggOre}},
    {418, {Ore::parhelionPlatinum, Ore::arcSandOre, Ore::everfreezeOre, Ore::boiledEggOre, Ore::marineRutile}},
    {419, {}},
    {420, {Ore::everfreezeOre}},
    {501, {Ore::arcSandOre}},
    {502, {Ore::bonjelium}},
    {503, {Ore::enduronLead}},
    {504, {Ore::arcSandOre, Ore::enduronLead, Ore::marineRutile, Ore::bonjelium}},
    {505, {}},
    {506, {Ore::bonjelium, Ore::arcSandOre}},
    {507, {Ore::bonjelium}},
    {508, {Ore::enduronLead, Ore::marineRutile}},
    {509, {}},
    {510, {Ore::bonjelium}},
    {511, {Ore::bonjelium}},
    {512, {}},
    {513, {}},
    {514, {}},
    {515, {}},
    {516, {}},
  };
  // clang-format on
  return oreMap.at(name);
}

std::vector<Site::Ptr> Site::getNeighbors() const {
  auto neighbors = Site::NEIGHBORS.at(name);
  // Don't return unvisited neighbors.
  std::erase_if(neighbors, [](const Site::Ptr &neighbor) {
    return !ProbeOptimizer::getSites().contains(neighbor);
  });
  return neighbors;
}

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
  default:
    break;
  }
  throw std::invalid_argument("Invalid grade");
}

char Site::gradeToChar(Grade grade) {
  switch (grade) {
  case Grade::F:
    return 'F';
  case Grade::E:
    return 'E';
  case Grade::D:
    return 'D';
  case Grade::C:
    return 'C';
  case Grade::B:
    return 'B';
  case Grade::A:
    return 'A';
  case Grade::S:
    return 'S';
  }
  throw std::invalid_argument("Invalid grade");
}

Site::Ptr Site::fromName(Id id) { return ALL.at(id); }

int Site::getProductionVal() const {
  switch (production) {
  case Grade::A:
    return 500;
  case Grade::B:
    return 350;
  case Grade::C:
    return 250;
  default:
    break;
  }
  throw std::invalid_argument("Invalid production grade");
}

int Site::getRevenueVal() const {
  switch (revenue) {
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
