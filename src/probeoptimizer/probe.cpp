//
// Created by preed on 12/27/15.
//

#include "probeoptimizer/probe.h"

// clang-format off
const std::map<Probe::Id, Probe> Probe::ALL{
  {"B", {.id = "B", .category = Category::Basic, .level = 0, .production = 0.5, .revenue = 0.5, .boost = 1}},
  {"M1", {.id = "M1", .category = Category::Mining, .level = 1, .production = 1, .revenue = 0.3, .boost = 1}},
  {"M2", {.id = "M2", .category = Category::Mining, .level = 2, .production = 1.2, .revenue = 0.3, .boost = 1}},
  {"M3", {.id = "M3", .category = Category::Mining, .level = 3, .production = 1.4, .revenue = 0.3, .boost = 1}},
  {"M4", {.id = "M4", .category = Category::Mining, .level = 4, .production = 1.6, .revenue = 0.3, .boost = 1}},
  {"M5", {.id = "M5", .category = Category::Mining, .level = 5, .production = 1.8, .revenue = 0.3, .boost = 1}},
  {"M6", {.id = "M6", .category = Category::Mining, .level = 6, .production = 2, .revenue = 0.3, .boost = 1}},
  {"M7", {.id = "M7", .category = Category::Mining, .level = 7, .production = 2.2, .revenue = 0.3, .boost = 1}},
  {"M8", {.id = "M8", .category = Category::Mining, .level = 8, .production = 2.4, .revenue = 0.3, .boost = 1}},
  {"M9", {.id = "M9", .category = Category::Mining, .level = 9, .production = 2.7, .revenue = 0.3, .boost = 1}},
  {"M10", {.id = "M10", .category = Category::Mining, .level = 10, .production = 3, .revenue = 0.3, .boost = 1}},
  {"R1", {.id = "R1", .category = Category::Research, .level = 1, .production = 0.3, .revenue = 2, .boost = 1}},
  {"R2", {.id = "R2", .category = Category::Research, .level = 2, .production = 0.3, .revenue = 2.5, .boost = 1}},
  {"R3", {.id = "R3", .category = Category::Research, .level = 3, .production = 0.3, .revenue = 3, .boost = 1}},
  {"R4", {.id = "R4", .category = Category::Research, .level = 4, .production = 0.3, .revenue = 3.5, .boost = 1}},
  {"R5", {.id = "R5", .category = Category::Research, .level = 5, .production = 0.3, .revenue = 4, .boost = 1}},
  {"R6", {.id = "R6", .category = Category::Research, .level = 6, .production = 0.3, .revenue = 4.5, .boost = 1}},
  {"B1", {.id = "B1", .category = Category::Booster, .level = 1, .production = 0.1, .revenue = 0.1, .boost = 1.5}},
  {"B2", {.id = "B2", .category = Category::Booster, .level = 2, .production = 0.1, .revenue = 0.1, .boost = 2}},
  {"D", {.id = "D", .category = Category::Duplicator, .level = 0, .production = 0, .revenue = 0, .boost = 1}},
  {"S", {.id = "S", .category = Category::Storage, .level = 0, .production = 0.1, .revenue = 0.1, .boost = 1}},
  {"CF", {.id = "CF", .category = Category::Battle, .level = 0, .production = 0, .revenue = 0, .boost = 1}},
  {"CM", {.id = "CM", .category = Category::Battle, .level = 0, .production = 0, .revenue = 0, .boost = 1}},
  {"CR", {.id = "CR", .category = Category::Battle, .level = 0, .production = 0, .revenue = 0, .boost = 1}},
  {"CD", {.id = "CD", .category = Category::Battle, .level = 0, .production = 0, .revenue = 0, .boost = 1}},
  {"CA", {.id = "CA", .category = Category::Battle, .level = 0, .production = 0, .revenue = 0, .boost = 1}},
};
// clang-format on

Probe::Ptr Probe::fromString(const std::string &s) {
  return &Probe::ALL.at(s);
}
