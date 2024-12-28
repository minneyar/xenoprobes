//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_ORE_H
#define XENOPROBES_ORE_H

#include <string>
#include <vector>

struct Ore {
  using Ptr = const Ore *;

  static const Ore::Ptr arcSandOre;
  static const Ore::Ptr aurorite;
  static const Ore::Ptr boiledEggOre;
  static const Ore::Ptr bonjelium;
  static const Ore::Ptr cimmerianCinnabar;
  static const Ore::Ptr dawnstone;
  static const Ore::Ptr enduronLead;
  static const Ore::Ptr everfreezeOre;
  static const Ore::Ptr foucaultium;
  static const Ore::Ptr infernium;
  static const Ore::Ptr lionboneBort;
  static const Ore::Ptr marineRutile;
  static const Ore::Ptr ouroborosCrystal;
  static const Ore::Ptr parhelionPlatinum;
  static const Ore::Ptr whiteCometite;

  static const std::vector<Ptr> ALL;

  std::string name;
  int importance;
};

#endif // XENOPROBES_ORE_H
