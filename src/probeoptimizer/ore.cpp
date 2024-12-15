//
// Created by preed on 1/6/16.
//

#include "probeoptimizer/ore.h"

const Ore Ore::arcSandOre{.name = "Arc Sand Ore", .importance = 0};
const Ore Ore::aurorite{.name = "Aurorite", .importance = 0};
const Ore Ore::boiledEggOre{.name = "Boiled-Egg Ore", .importance = 0};
const Ore Ore::bonjelium{.name = "Bonjelium", .importance = 0};
const Ore Ore::cimmerianCinnabar{.name = "Cimmerian Cinnabar", .importance = 0};
const Ore Ore::dawnstone{.name = "Dawnstone", .importance = 0};
const Ore Ore::enduronLead{.name = "Enduron Lead", .importance = 0};
const Ore Ore::everfreezeOre{.name = "Everfreeze Ore", .importance = 0};
const Ore Ore::foucaultium{.name = "Foucaultium", .importance = 0};
const Ore Ore::infernium{.name = "Infernium", .importance = 0};
const Ore Ore::lionboneBort{.name = "Lionbone Bort", .importance = 0};
const Ore Ore::marineRutile{.name = "Marine Rutile", .importance = 0};
const Ore Ore::ouroborosCrystal{.name = "Ouroboros Crystal", .importance = 0};
const Ore Ore::parhelionPlatinum{.name = "Parhelion Platinum", .importance = 0};
const Ore Ore::whiteCometite{.name = "White Cometite", .importance = 0};

const std::vector<Ore::Ptr> Ore::ALL{
    &arcSandOre,        &aurorite,          &boiledEggOre,  &bonjelium,
    &cimmerianCinnabar, &dawnstone,         &enduronLead,   &everfreezeOre,
    &foucaultium,       &infernium,         &lionboneBort,  &marineRutile,
    &ouroborosCrystal,  &parhelionPlatinum, &whiteCometite,
};
