//
// Created by preed on 1/6/16.
//

#include "probeoptimizer/ore.h"

// clang-format off
const Ore::Ptr Ore::arcSandOre = new Ore{.name = "Arc Sand Ore", .importance = 0};
const Ore::Ptr Ore::aurorite = new Ore{.name = "Aurorite", .importance = 0};
const Ore::Ptr Ore::boiledEggOre = new Ore{.name = "Boiled-Egg Ore", .importance = 0};
const Ore::Ptr Ore::bonjelium = new Ore{.name = "Bonjelium", .importance = 0};
const Ore::Ptr Ore::cimmerianCinnabar = new Ore{.name = "Cimmerian Cinnabar", .importance = 0};
const Ore::Ptr Ore::dawnstone = new Ore{.name = "Dawnstone", .importance = 0};
const Ore::Ptr Ore::enduronLead = new Ore{.name = "Enduron Lead", .importance = 0};
const Ore::Ptr Ore::everfreezeOre = new Ore{.name = "Everfreeze Ore", .importance = 0};
const Ore::Ptr Ore::foucaultium = new Ore{.name = "Foucaultium", .importance = 0};
const Ore::Ptr Ore::infernium = new Ore{.name = "Infernium", .importance = 0};
const Ore::Ptr Ore::lionboneBort = new Ore{.name = "Lionbone Bort", .importance = 0};
const Ore::Ptr Ore::marineRutile = new Ore{.name = "Marine Rutile", .importance = 0};
const Ore::Ptr Ore::ouroborosCrystal = new Ore{.name = "Ouroboros Crystal", .importance = 0};
const Ore::Ptr Ore::parhelionPlatinum = new Ore{.name = "Parhelion Platinum", .importance = 0};
const Ore::Ptr Ore::whiteCometite = new Ore{.name = "White Cometite", .importance = 0};
// // clang-format on

const std::vector<Ore::Ptr> Ore::ALL{
    arcSandOre,        aurorite,          boiledEggOre,  bonjelium,
    cimmerianCinnabar, dawnstone,         enduronLead,   everfreezeOre,
    foucaultium,       infernium,         lionboneBort,  marineRutile,
    ouroborosCrystal,  parhelionPlatinum, whiteCometite,
};
