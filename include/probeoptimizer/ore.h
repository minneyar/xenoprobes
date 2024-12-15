//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_ORE_H
#define XENOPROBES_ORE_H

#include <string>
#include <vector>

struct Ore {
  using Ptr = const Ore *;

  static const Ore arcSandOre;
  static const Ore aurorite;
  static const Ore boiledEggOre;
  static const Ore bonjelium;
  static const Ore cimmerianCinnabar;
  static const Ore dawnstone;
  static const Ore enduronLead;
  static const Ore everfreezeOre;
  static const Ore foucaultium;
  static const Ore infernium;
  static const Ore lionboneBort;
  static const Ore marineRutile;
  static const Ore ouroborosCrystal;
  static const Ore parhelionPlatinum;
  static const Ore whiteCometite;

  static const std::vector<Ptr> ALL;

  std::string name;
  int importance;
};

#endif // XENOPROBES_ORE_H
