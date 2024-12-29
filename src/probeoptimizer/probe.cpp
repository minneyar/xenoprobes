//
// Created by preed on 12/27/15.
//

#include "probeoptimizer/probe.h"

// clang-format off
const Probe::Ptr Probe::B = new Probe{.id = "B", .category = Category::Basic, .level = 0, .production = 0.5, .revenue = 0.5, .boost = 1};
const Probe::Ptr Probe::M1 = new Probe{.id = "M1", .category = Category::Mining, .level = 1, .production = 1, .revenue = 0.3, .boost = 1};
const Probe::Ptr Probe::M2 = new Probe{.id = "M2", .category = Category::Mining, .level = 2, .production = 1.2, .revenue = 0.3, .boost = 1};
const Probe::Ptr Probe::M3 = new Probe{.id = "M3", .category = Category::Mining, .level = 3, .production = 1.4, .revenue = 0.3, .boost = 1};
const Probe::Ptr Probe::M4 = new Probe{.id = "M4", .category = Category::Mining, .level = 4, .production = 1.6, .revenue = 0.3, .boost = 1};
const Probe::Ptr Probe::M5 = new Probe{.id = "M5", .category = Category::Mining, .level = 5, .production = 1.8, .revenue = 0.3, .boost = 1};
const Probe::Ptr Probe::M6 = new Probe{.id = "M6", .category = Category::Mining, .level = 6, .production = 2, .revenue = 0.3, .boost = 1};
const Probe::Ptr Probe::M7 = new Probe{.id = "M7", .category = Category::Mining, .level = 7, .production = 2.2, .revenue = 0.3, .boost = 1};
const Probe::Ptr Probe::M8 = new Probe{.id = "M8", .category = Category::Mining, .level = 8, .production = 2.4, .revenue = 0.3, .boost = 1};
const Probe::Ptr Probe::M9 = new Probe{.id = "M9", .category = Category::Mining, .level = 9, .production = 2.7, .revenue = 0.3, .boost = 1};
const Probe::Ptr Probe::M10 = new Probe{.id = "M10", .category = Category::Mining, .level = 10, .production = 3, .revenue = 0.3, .boost = 1};
const Probe::Ptr Probe::R1 = new Probe{.id = "R1", .category = Category::Research, .level = 1, .production = 0.3, .revenue = 2, .boost = 1};
const Probe::Ptr Probe::R2 = new Probe{.id = "R2", .category = Category::Research, .level = 2, .production = 0.3, .revenue = 2.5, .boost = 1};
const Probe::Ptr Probe::R3 = new Probe{.id = "R3", .category = Category::Research, .level = 3, .production = 0.3, .revenue = 3, .boost = 1};
const Probe::Ptr Probe::R4 = new Probe{.id = "R4", .category = Category::Research, .level = 4, .production = 0.3, .revenue = 3.5, .boost = 1};
const Probe::Ptr Probe::R5 = new Probe{.id = "R5", .category = Category::Research, .level = 5, .production = 0.3, .revenue = 4, .boost = 1};
const Probe::Ptr Probe::R6 = new Probe{.id = "R6", .category = Category::Research, .level = 6, .production = 0.3, .revenue = 4.5, .boost = 1};
const Probe::Ptr Probe::B1 = new Probe{.id = "B1", .category = Category::Booster, .level = 1, .production = 0.1, .revenue = 0.1, .boost = 1.5};
const Probe::Ptr Probe::B2 = new Probe{.id = "B2", .category = Category::Booster, .level = 2, .production = 0.1, .revenue = 0.1, .boost = 2};
const Probe::Ptr Probe::D = new Probe{.id = "D", .category = Category::Duplicator, .level = 0, .production = 0, .revenue = 0, .boost = 1};
const Probe::Ptr Probe::S = new Probe{.id = "S", .category = Category::Storage, .level = 0, .production = 0.1, .revenue = 0.1, .boost = 1};
const Probe::Ptr Probe::CF = new Probe{.id = "CF", .category = Category::Battle, .level = 0, .production = 0, .revenue = 0, .boost = 1};
const Probe::Ptr Probe::CM = new Probe{.id = "CM", .category = Category::Battle, .level = 0, .production = 0, .revenue = 0, .boost = 1};
const Probe::Ptr Probe::CR = new Probe{.id = "CR", .category = Category::Battle, .level = 0, .production = 0, .revenue = 0, .boost = 1};
const Probe::Ptr Probe::CD = new Probe{.id = "CD", .category = Category::Battle, .level = 0, .production = 0, .revenue = 0, .boost = 1};
const Probe::Ptr Probe::CA = new Probe{.id = "CA", .category = Category::Battle, .level = 0, .production = 0, .revenue = 0, .boost = 1};
// clang-format on

const std::unordered_map<Probe::Id, Probe::Ptr> Probe::ALL{
    {"B", Probe::B},   {"M1", Probe::M1},   {"M2", Probe::M2},
    {"M3", Probe::M3}, {"M4", Probe::M4},   {"M5", Probe::M5},
    {"M6", Probe::M6}, {"M7", Probe::M7},   {"M8", Probe::M8},
    {"M9", Probe::M9}, {"M10", Probe::M10}, {"R1", Probe::R1},
    {"R2", Probe::R2}, {"R3", Probe::R3},   {"R4", Probe::R4},
    {"R5", Probe::R5}, {"R6", Probe::R6},   {"B1", Probe::B1},
    {"B2", Probe::B2}, {"D", Probe::D},     {"S", Probe::S},
    {"CF", Probe::CF}, {"CM", Probe::CM},   {"CR", Probe::CR},
    {"CD", Probe::CD}, {"CA", Probe::CA},
};
const std::vector<Probe::Ptr> Probe::ALL_SORTED{
    Probe::B,  Probe::M1, Probe::M2, Probe::M3, Probe::M4,  Probe::M5,
    Probe::M6, Probe::M7, Probe::M8, Probe::M9, Probe::M10, Probe::R1,
    Probe::R2, Probe::R3, Probe::R4, Probe::R5, Probe::R6,  Probe::B1,
    Probe::B2, Probe::D,  Probe::S,  Probe::CF, Probe::CM,  Probe::CR,
    Probe::CD, Probe::CA,
};

Probe::Ptr Probe::fromString(const std::string &s) { return Probe::ALL.at(s); }
