//
// Created by preed on 1/6/16.
//

#include "probeoptimizer/site.h"

#include <probeoptimizer/probe_optimizer.h>
#include <spdlog/spdlog.h>

const std::map<Site::Id, Site> Site::ALL{
    {101,
     {.name = 101,
      .position = {1129, 2974},
      .production = Grade::C,
      .revenue = Grade::S,
      .combat = Grade::S,
      .sightseeing = 1}},
    {102,
     {.name = 102,
      .position = {1129, 3656},
      .production = Grade::C,
      .revenue = Grade::F,
      .combat = Grade::B,
      .sightseeing = 0}},
    {103,
     {.name = 103,
      .position = {1129, 3199},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::A,
      .sightseeing = 1}},
    {104,
     {.name = 104,
      .position = {1213, 3485},
      .production = Grade::C,
      .revenue = Grade::S,
      .combat = Grade::B,
      .sightseeing = 1}},
    {105,
     {.name = 105,
      .position = {1298, 2974},
      .production = Grade::A,
      .revenue = Grade::F,
      .combat = Grade::B,
      .sightseeing = 0}},
    {106,
     {.name = 106,
      .position = {1297, 3314},
      .production = Grade::B,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 1}},
    {107,
     {.name = 107,
      .position = {1379, 3600},
      .production = Grade::A,
      .revenue = Grade::F,
      .combat = Grade::B,
      .sightseeing = 0}},
    {108,
     {.name = 108,
      .position = {1470, 3199},
      .production = Grade::C,
      .revenue = Grade::F,
      .combat = Grade::B,
      .sightseeing = 0}},
    {109,
     {.name = 109,
      .position = {1551, 3030},
      .production = Grade::C,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 0}},
    {110,
     {.name = 110,
      .position = {1551, 3367},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 1}},
    {111,
     {.name = 111,
      .position = {1636, 3088},
      .production = Grade::C,
      .revenue = Grade::F,
      .combat = Grade::B,
      .sightseeing = 0}},
    {112,
     {.name = 112,
      .position = {1721, 3367},
      .production = Grade::A,
      .revenue = Grade::F,
      .combat = Grade::A,
      .sightseeing = 0}},
    {113,
     {.name = 113,
      .position = {1804, 2861},
      .production = Grade::C,
      .revenue = Grade::C,
      .combat = Grade::B,
      .sightseeing = 0}},
    {114,
     {.name = 114,
      .position = {1804, 3542},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 0}},
    {115,
     {.name = 115,
      .position = {1891, 3258},
      .production = Grade::C,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 0}},
    {116,
     {.name = 116,
      .position = {1974, 3656},
      .production = Grade::A,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 0}},
    {117,
     {.name = 117,
      .position = {2060, 3487},
      .production = Grade::A,
      .revenue = Grade::D,
      .combat = Grade::A,
      .sightseeing = 1}},
    {118,
     {.name = 118,
      .position = {2229, 3367},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 0}},
    {119,
     {.name = 119,
      .position = {2229, 3710},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 0}},
    {120,
     {.name = 120,
      .position = {2320, 3542},
      .production = Grade::B,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 0}},
    {121,
     {.name = 121,
      .position = {2404, 3375},
      .production = Grade::A,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 0}},
    {201,
     {.name = 201,
      .position = {337, 1434},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::S,
      .sightseeing = 0}},
    {202,
     {.name = 202,
      .position = {337, 1774},
      .production = Grade::C,
      .revenue = Grade::C,
      .combat = Grade::B,
      .sightseeing = 0}},
    {203,
     {.name = 203,
      .position = {337, 1999},
      .production = Grade::C,
      .revenue = Grade::A,
      .combat = Grade::B,
      .sightseeing = 0}},
    {204,
     {.name = 204,
      .position = {418, 2167},
      .production = Grade::A,
      .revenue = Grade::C,
      .combat = Grade::B,
      .sightseeing = 0}},
    {205,
     {.name = 205,
      .position = {418, 2390},
      .production = Grade::A,
      .revenue = Grade::F,
      .combat = Grade::B,
      .sightseeing = 0}},
    {206,
     {.name = 206,
      .position = {505, 1320},
      .production = Grade::B,
      .revenue = Grade::A,
      .combat = Grade::S,
      .sightseeing = 0}},
    {207,
     {.name = 207,
      .position = {505, 1660},
      .production = Grade::C,
      .revenue = Grade::C,
      .combat = Grade::B,
      .sightseeing = 0}},
    {208,
     {.name = 208,
      .position = {505, 1886},
      .production = Grade::B,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 0}},
    {209,
     {.name = 209,
      .position = {506, 2558},
      .production = Grade::C,
      .revenue = Grade::F,
      .combat = Grade::B,
      .sightseeing = 0}},
    {210,
     {.name = 210,
      .position = {593, 1829},
      .production = Grade::B,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 0}},
    {211,
     {.name = 211,
      .position = {593, 2055},
      .production = Grade::A,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 0}},
    {212,
     {.name = 212,
      .position = {593, 2281},
      .production = Grade::B,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 0}},
    {213,
     {.name = 213,
      .position = {674, 1434},
      .production = Grade::C,
      .revenue = Grade::S,
      .combat = Grade::B,
      .sightseeing = 1}},
    {214,
     {.name = 214,
      .position = {674, 1886},
      .production = Grade::C,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 2}},
    {215,
     {.name = 215,
      .position = {674, 2112},
      .production = Grade::C,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 0}},
    {216,
     {.name = 216,
      .position = {674, 2444},
      .production = Grade::C,
      .revenue = Grade::A,
      .combat = Grade::A,
      .sightseeing = 1}},
    {217,
     {.name = 217,
      .position = {674, 3011},
      .production = Grade::C,
      .revenue = Grade::C,
      .combat = Grade::B,
      .sightseeing = 0}},
    {218,
     {.name = 218,
      .position = {761, 2281},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 0}},
    {219,
     {.name = 219,
      .position = {760, 2837},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 0}},
    {220,
     {.name = 220,
      .position = {845, 2670},
      .production = Grade::C,
      .revenue = Grade::C,
      .combat = Grade::A,
      .sightseeing = 1}},
    {221,
     {.name = 221,
      .position = {842, 2896},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 2}},
    {222,
     {.name = 222,
      .position = {842, 3122},
      .production = Grade::C,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 1}},
    {223,
     {.name = 223,
      .position = {930, 2055},
      .production = Grade::C,
      .revenue = Grade::F,
      .combat = Grade::B,
      .sightseeing = 1}},
    {224,
     {.name = 224,
      .position = {930, 2167},
      .production = Grade::C,
      .revenue = Grade::A,
      .combat = Grade::B,
      .sightseeing = 0}},
    {225,
     {.name = 225,
      .position = {930, 2503},
      .production = Grade::C,
      .revenue = Grade::A,
      .combat = Grade::B,
      .sightseeing = 1}},
    {301,
     {.name = 301,
      .position = {2514, 3441},
      .production = Grade::B,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 0}},
    {302,
     {.name = 302,
      .position = {2682, 3328},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 0}},
    {303,
     {.name = 303,
      .position = {2682, 3664},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 0}},
    {304,
     {.name = 304,
      .position = {2765, 3495},
      .production = Grade::B,
      .revenue = Grade::A,
      .combat = Grade::S,
      .sightseeing = 0}},
    {305,
     {.name = 305,
      .position = {2849, 3216},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 0}},
    {306,
     {.name = 306,
      .position = {2934, 3608},
      .production = Grade::C,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 1}},
    {307,
     {.name = 307,
      .position = {2934, 3831},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 0}},
    {308,
     {.name = 308,
      .position = {3017, 2993},
      .production = Grade::B,
      .revenue = Grade::C,
      .combat = Grade::A,
      .sightseeing = 0}},
    {309,
     {.name = 309,
      .position = {3017, 3328},
      .production = Grade::C,
      .revenue = Grade::C,
      .combat = Grade::B,
      .sightseeing = 0}},
    {310,
     {.name = 310,
      .position = {3185, 2993},
      .production = Grade::C,
      .revenue = Grade::A,
      .combat = Grade::B,
      .sightseeing = 0}},
    {311,
     {.name = 311,
      .position = {3185, 3216},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 0}},
    {312,
     {.name = 312,
      .position = {3185, 3552},
      .production = Grade::C,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 0}},
    {313,
     {.name = 313,
      .position = {3185, 3776},
      .production = Grade::C,
      .revenue = Grade::E,
      .combat = Grade::A,
      .sightseeing = 2}},
    {314,
     {.name = 314,
      .position = {3185, 4000},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::S,
      .sightseeing = 0}},
    {315,
     {.name = 315,
      .position = {3269, 3273},
      .production = Grade::A,
      .revenue = Grade::S,
      .combat = Grade::B,
      .sightseeing = 2}},
    {316,
     {.name = 316,
      .position = {3353, 2991},
      .production = Grade::C,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 0}},
    {317,
     {.name = 317,
      .position = {3353, 3775},
      .production = Grade::C,
      .revenue = Grade::A,
      .combat = Grade::B,
      .sightseeing = 1}},
    {318,
     {.name = 318,
      .position = {3436, 3495},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 2}},
    {319,
     {.name = 319,
      .position = {3436, 3830},
      .production = Grade::C,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 1}},
    {320,
     {.name = 320,
      .position = {3520, 2991},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 0}},
    {321,
     {.name = 321,
      .position = {3520, 3216},
      .production = Grade::A,
      .revenue = Grade::D,
      .combat = Grade::A,
      .sightseeing = 0}},
    {322,
     {.name = 322,
      .position = {3688, 3440},
      .production = Grade::A,
      .revenue = Grade::A,
      .combat = Grade::B,
      .sightseeing = 0}},
    {401,
     {.name = 401,
      .position = {2012, 1738},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 0}},
    {402,
     {.name = 402,
      .position = {2099, 1909},
      .production = Grade::A,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 0}},
    {403,
     {.name = 403,
      .position = {2099, 2245},
      .production = Grade::A,
      .revenue = Grade::C,
      .combat = Grade::S,
      .sightseeing = 0}},
    {404,
     {.name = 404,
      .position = {2184, 1514},
      .production = Grade::B,
      .revenue = Grade::S,
      .combat = Grade::S,
      .sightseeing = 1}},
    {405,
     {.name = 405,
      .position = {2184, 2077},
      .production = Grade::A,
      .revenue = Grade::E,
      .combat = Grade::A,
      .sightseeing = 0}},
    {406,
     {.name = 406,
      .position = {2268, 1681},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 0}},
    {407,
     {.name = 407,
      .position = {2354, 1401},
      .production = Grade::A,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 0}},
    {408,
     {.name = 408,
      .position = {2352, 1852},
      .production = Grade::B,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 1}},
    {409,
     {.name = 409,
      .position = {2352, 2304},
      .production = Grade::B,
      .revenue = Grade::S,
      .combat = Grade::B,
      .sightseeing = 0}},
    {410,
     {.name = 410,
      .position = {2438, 1229},
      .production = Grade::C,
      .revenue = Grade::S,
      .combat = Grade::B,
      .sightseeing = 1}},
    {411,
     {.name = 411,
      .position = {2438, 2021},
      .production = Grade::A,
      .revenue = Grade::A,
      .combat = Grade::S,
      .sightseeing = 0}},
    {412,
     {.name = 412,
      .position = {2524, 1514},
      .production = Grade::A,
      .revenue = Grade::B,
      .combat = Grade::A,
      .sightseeing = 0}},
    {413,
     {.name = 413,
      .position = {2522, 1738},
      .production = Grade::C,
      .revenue = Grade::A,
      .combat = Grade::B,
      .sightseeing = 1}},
    {414,
     {.name = 414,
      .position = {2606, 2131},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 2}},
    {415,
     {.name = 415,
      .position = {2692, 1285},
      .production = Grade::C,
      .revenue = Grade::S,
      .combat = Grade::B,
      .sightseeing = 0}},
    {416,
     {.name = 416,
      .position = {2692, 1624},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 0}},
    {417,
     {.name = 417,
      .position = {2692, 1964},
      .production = Grade::B,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 0}},
    {418,
     {.name = 418,
      .position = {2776, 1455},
      .production = Grade::C,
      .revenue = Grade::C,
      .combat = Grade::B,
      .sightseeing = 0}},
    {419,
     {.name = 419,
      .position = {2776, 1794},
      .production = Grade::C,
      .revenue = Grade::S,
      .combat = Grade::S,
      .sightseeing = 1}},
    {420,
     {.name = 420,
      .position = {2944, 1681},
      .production = Grade::B,
      .revenue = Grade::C,
      .combat = Grade::B,
      .sightseeing = 0}},
    {501,
     {.name = 501,
      .position = {2606, 650},
      .production = Grade::B,
      .revenue = Grade::F,
      .combat = Grade::B,
      .sightseeing = 0}},
    {502,
     {.name = 502,
      .position = {2606, 990},
      .production = Grade::A,
      .revenue = Grade::C,
      .combat = Grade::B,
      .sightseeing = 1}},
    {503,
     {.name = 503,
      .position = {2776, 766},
      .production = Grade::C,
      .revenue = Grade::D,
      .combat = Grade::B,
      .sightseeing = 1}},
    {504,
     {.name = 504,
      .position = {2860, 1048},
      .production = Grade::C,
      .revenue = Grade::C,
      .combat = Grade::B,
      .sightseeing = 0}},
    {505,
     {.name = 505,
      .position = {2946, 650},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 2}},
    {506,
     {.name = 506,
      .position = {2946, 875},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 1}},
    {507,
     {.name = 507,
      .position = {3028, 1268},
      .production = Grade::C,
      .revenue = Grade::A,
      .combat = Grade::B,
      .sightseeing = 1}},
    {508,
     {.name = 508,
      .position = {3111, 990},
      .production = Grade::A,
      .revenue = Grade::B,
      .combat = Grade::S,
      .sightseeing = 1}},
    {509,
     {.name = 509,
      .position = {3197, 706},
      .production = Grade::A,
      .revenue = Grade::A,
      .combat = Grade::A,
      .sightseeing = 0}},
    {510,
     {.name = 510,
      .position = {3282, 422},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::B,
      .sightseeing = 0}},
    {511,
     {.name = 511,
      .position = {3282, 990},
      .production = Grade::A,
      .revenue = Grade::C,
      .combat = Grade::A,
      .sightseeing = 0}},
    {512,
     {.name = 512,
      .position = {3282, 1213},
      .production = Grade::C,
      .revenue = Grade::A,
      .combat = Grade::S,
      .sightseeing = 0}},
    {513,
     {.name = 513,
      .position = {3367, 706},
      .production = Grade::C,
      .revenue = Grade::A,
      .combat = Grade::B,
      .sightseeing = 2}},
    {514,
     {.name = 514,
      .position = {3450, 1099},
      .production = Grade::C,
      .revenue = Grade::A,
      .combat = Grade::B,
      .sightseeing = 1}},
    {515,
     {.name = 515,
      .position = {3535, 931},
      .production = Grade::C,
      .revenue = Grade::B,
      .combat = Grade::S,
      .sightseeing = 0}},
    {516,
     {.name = 516,
      .position = {3620, 650},
      .production = Grade::B,
      .revenue = Grade::E,
      .combat = Grade::B,
      .sightseeing = 0}},
};

const std::vector<Ore::Ptr> &Site::getOre() const {
  // clang-format off
  static const std::unordered_map<Id, std::vector<Ore::Ptr>> oreMap{
    {101, {}},
    {102, {}},
    {103, {}},
    {104, {}},
    {105, {}},
    {106, {&Ore::arcSandOre}},
    {107, {}},
    {108, {&Ore::aurorite, &Ore::arcSandOre, &Ore::foucaultium}},
    {109, {&Ore::foucaultium, &Ore::dawnstone, &Ore::lionboneBort}},
    {110, {&Ore::aurorite, &Ore::arcSandOre, &Ore::whiteCometite, &Ore::dawnstone}},
    {111, {&Ore::foucaultium}},
    {112, {}},
    {113, {}},
    {114, {}},
    {115, {&Ore::arcSandOre, &Ore::whiteCometite, &Ore::lionboneBort}},
    {116, {}},
    {117, {}},
    {118, {&Ore::aurorite, &Ore::whiteCometite, &Ore::dawnstone}},
    {119, {}},
    {120, {}},
    {121, {}},
    {201, {}},
    {202, {&Ore::cimmerianCinnabar, &Ore::everfreezeOre}},
    {203, {&Ore::cimmerianCinnabar}},
    {204, {}},
    {205, {}},
    {206, {}},
    {207, {&Ore::infernium, &Ore::whiteCometite, &Ore::cimmerianCinnabar, &Ore::foucaultium}},
    {208, {&Ore::foucaultium}},
    {209, {}},
    {210, {}},
    {211, {}},
    {212, {&Ore::aurorite, &Ore::enduronLead, &Ore::whiteCometite}},
    {213, {}},
    {214, {}},
    {215, {&Ore::aurorite, &Ore::enduronLead, &Ore::everfreezeOre}},
    {216, {}},
    {217, {&Ore::aurorite, &Ore::infernium, &Ore::cimmerianCinnabar}},
    {218, {&Ore::aurorite, &Ore::enduronLead, &Ore::whiteCometite}},
    {219, {&Ore::enduronLead, &Ore::whiteCometite}},
    {220, {&Ore::infernium, &Ore::everfreezeOre}},
    {221, {}},
    {222, {}},
    {223, {}},
    {224, {}},
    {225, {}},
    {301, {&Ore::infernium, &Ore::arcSandOre, &Ore::lionboneBort}},
    {302, {}},
    {303, {&Ore::aurorite, &Ore::whiteCometite}},
    {304, {}},
    {305, {&Ore::aurorite, &Ore::arcSandOre, &Ore::enduronLead}},
    {306, {}},
    {307, {&Ore::infernium, &Ore::arcSandOre, &Ore::enduronLead, &Ore::whiteCometite}},
    {308, {&Ore::ouroborosCrystal}},
    {309, {&Ore::enduronLead, &Ore::ouroborosCrystal}},
    {310, {}},
    {311, {}},
    {312, {&Ore::infernium, &Ore::boiledEggOre, &Ore::lionboneBort}},
    {313, {}},
    {314, {}},
    {315, {}},
    {316, {}},
    {317, {}},
    {318, {&Ore::boiledEggOre, &Ore::whiteCometite, &Ore::lionboneBort}},
    {319, {&Ore::infernium, &Ore::boiledEggOre}},
    {320, {&Ore::aurorite, &Ore::ouroborosCrystal}},
    {321, {}},
    {322, {}},
    {401, {&Ore::parhelionPlatinum, &Ore::marineRutile}},
    {402, {}},
    {403, {}},
    {404, {}},
    {405, {&Ore::arcSandOre}},
    {406, {}},
    {407, {}},
    {408, {&Ore::aurorite, &Ore::arcSandOre, &Ore::everfreezeOre}},
    {409, {}},
    {410, {}},
    {411, {}},
    {412, {}},
    {413, {}},
    {414, {&Ore::parhelionPlatinum, &Ore::marineRutile}},
    {415, {}},
    {416, {}},
    {417, {&Ore::everfreezeOre, &Ore::boiledEggOre}},
    {418, {&Ore::parhelionPlatinum, &Ore::arcSandOre, &Ore::everfreezeOre, &Ore::boiledEggOre, &Ore::marineRutile}},
    {419, {}},
    {420, {&Ore::everfreezeOre}},
    {501, {&Ore::arcSandOre}},
    {502, {&Ore::bonjelium}},
    {503, {&Ore::enduronLead}},
    {504, {&Ore::arcSandOre, &Ore::enduronLead, &Ore::marineRutile, &Ore::bonjelium}},
    {505, {}},
    {506, {&Ore::bonjelium, &Ore::arcSandOre}},
    {507, {&Ore::bonjelium}},
    {508, {&Ore::enduronLead, &Ore::marineRutile}},
    {509, {}},
    {510, {&Ore::bonjelium}},
    {511, {&Ore::bonjelium}},
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
  static const std::unordered_map<Id, std::vector<Ptr>> neighborsMap{
      {101, {fromName(105)}},
      {102, {fromName(104)}},
      {103, {fromName(106), fromName(105), fromName(222)}},
      {104, {fromName(102), fromName(106)}},
      {105, {fromName(101), fromName(103), fromName(109)}},
      {106, {fromName(103), fromName(104), fromName(107)}},
      {107, {fromName(106), fromName(110)}},
      {108, {fromName(109)}},
      {109, {fromName(105), fromName(108)}},
      {110, {fromName(107), fromName(111), fromName(112)}},
      {111, {fromName(110), fromName(113)}},
      {112, {fromName(110), fromName(114), fromName(115)}},
      {113, {fromName(111), fromName(409)}},
      {114, {fromName(112), fromName(116)}},
      {115, {fromName(112)}},
      {116, {fromName(114), fromName(117)}},
      {117, {fromName(116), fromName(118), fromName(120)}},
      {118, {fromName(117), fromName(121)}},
      {119, {fromName(120)}},
      {120, {fromName(117), fromName(119)}},
      {121, {fromName(118), fromName(301)}},
      {201, {fromName(206)}},
      {202, {fromName(207), fromName(203), fromName(208)}},
      {203, {fromName(202), fromName(204)}},
      {204, {fromName(203), fromName(205), fromName(211), fromName(212)}},
      {205, {fromName(204), fromName(209)}},
      {206, {fromName(201), fromName(207), fromName(213)}},
      {207, {fromName(202), fromName(206)}},
      {208, {fromName(202)}},
      {209, {fromName(205)}},
      {210, {fromName(211)}},
      {211, {fromName(204), fromName(210)}},
      {212, {fromName(204), fromName(216)}},
      {213, {fromName(206)}},
      {214, {fromName(215)}},
      {215, {fromName(214), fromName(218)}},
      {216, {fromName(212), fromName(218), fromName(225)}},
      {217, {fromName(222)}},
      {218, {fromName(215), fromName(216), fromName(224)}},
      {219, {fromName(220)}},
      {220, {fromName(219), fromName(221), fromName(225)}},
      {221, {fromName(220), fromName(222)}},
      {222, {fromName(217), fromName(221), fromName(103)}},
      {223, {fromName(224)}},
      {224, {fromName(218), fromName(223)}},
      {225, {fromName(216), fromName(220)}},
      {301, {fromName(302), fromName(303), fromName(121)}},
      {302, {fromName(301)}},
      {303, {fromName(301), fromName(306)}},
      {304, {fromName(305), fromName(306), fromName(309)}},
      {305, {fromName(304), fromName(308)}},
      {306, {fromName(303), fromName(304), fromName(307)}},
      {307, {fromName(306), fromName(313)}},
      {308, {fromName(305)}},
      {309, {fromName(304), fromName(311)}},
      {310, {fromName(311)}},
      {311, {fromName(309), fromName(310)}},
      {312, {fromName(315), fromName(313)}},
      {313, {fromName(307), fromName(312), fromName(314)}},
      {314, {fromName(313)}},
      {315, {fromName(312), fromName(316), fromName(318), fromName(321)}},
      {316, {fromName(315)}},
      {317, {fromName(318), fromName(319)}},
      {318, {fromName(315), fromName(317)}},
      {319, {fromName(317)}},
      {320, {fromName(321)}},
      {321, {fromName(315), fromName(320), fromName(322)}},
      {322, {fromName(321)}},
      {401, {fromName(402), fromName(404)}},
      {402, {fromName(401), fromName(408)}},
      {403, {fromName(405)}},
      {404, {fromName(401), fromName(407)}},
      {405, {fromName(403), fromName(408), fromName(409)}},
      {406, {fromName(408)}},
      {407, {fromName(404), fromName(412)}},
      {408, {fromName(402), fromName(405), fromName(406), fromName(413)}},
      {409, {fromName(405), fromName(411), fromName(113)}},
      {410, {fromName(412)}},
      {411, {fromName(409), fromName(414)}},
      {412, {fromName(407), fromName(410), fromName(415)}},
      {413, {fromName(408), fromName(416)}},
      {414, {fromName(411)}},
      {415, {fromName(412), fromName(502)}},
      {416, {fromName(413), fromName(418), fromName(419)}},
      {417, {fromName(419)}},
      {418, {fromName(416)}},
      {419, {fromName(416), fromName(417), fromName(420)}},
      {420, {fromName(419)}},
      {501, {fromName(502)}},
      {502, {fromName(501), fromName(503), fromName(415)}},
      {503, {fromName(502), fromName(504)}},
      {504, {fromName(503), fromName(508)}},
      {505, {fromName(506), fromName(509)}},
      {506, {fromName(505)}},
      {507, {fromName(508)}},
      {508, {fromName(504), fromName(507), fromName(509), fromName(511)}},
      {509, {fromName(505), fromName(508), fromName(510), fromName(513)}},
      {510, {fromName(509)}},
      {511, {fromName(508), fromName(512), fromName(514)}},
      {512, {fromName(511)}},
      {513, {fromName(509), fromName(516)}},
      {514, {fromName(511), fromName(515)}},
      {515, {fromName(514)}},
      {516, {fromName(513)}},
  };

  auto neighbors = neighborsMap.at(name);
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

Site::Ptr Site::fromName(Id id) { return &ALL.at(id); }

int Site::getProductionVal() const {
  switch (production) {
  case Grade::A:
    return 500;
  case Grade::B:
    return 350;
  case Grade::C:
    return 250;
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
