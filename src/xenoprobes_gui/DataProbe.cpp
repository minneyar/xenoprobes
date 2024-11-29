/**
 * @file FnProbe.cpp
 *
 * @author Dan Keenan
 * @date 11/24/24
 * @copyright GNU GPLv3
 */

#include "DataProbe.h"

const QHash<DataProbe::Id, DataProbe> DataProbe::kAllProbes{
    {"M1", {.id = "M1", .category = DataProbe::Category::Mining, .level = 1, .name = "Mining Probe G1"}},
    {"M2", {.id = "M2", .category = DataProbe::Category::Mining, .level = 2, .name = "Mining Probe G2"}},
    {"M3", {.id = "M3", .category = DataProbe::Category::Mining, .level = 3, .name = "Mining Probe G3"}},
    {"M4", {.id = "M4", .category = DataProbe::Category::Mining, .level = 4, .name = "Mining Probe G4"}},
    {"M5", {.id = "M5", .category = DataProbe::Category::Mining, .level = 5, .name = "Mining Probe G5"}},
    {"M6", {.id = "M6", .category = DataProbe::Category::Mining, .level = 6, .name = "Mining Probe G6"}},
    {"M7", {.id = "M7", .category = DataProbe::Category::Mining, .level = 7, .name = "Mining Probe G7"}},
    {"M8", {.id = "M8", .category = DataProbe::Category::Mining, .level = 8, .name = "Mining Probe G8"}},
    {"M9", {.id = "M9", .category = DataProbe::Category::Mining, .level = 9, .name = "Mining Probe G9"}},
    {"M10", {.id = "M10", .category = DataProbe::Category::Mining, .level = 10, .name = "Mining Probe G10"}},
    {"R1", {.id = "R1", .category = DataProbe::Category::Research, .level = 1, .name = "Research Probe G1"}},
    {"R2", {.id = "R2", .category = DataProbe::Category::Research, .level = 2, .name = "Research Probe G2"}},
    {"R3", {.id = "R3", .category = DataProbe::Category::Research, .level = 3, .name = "Research Probe G3"}},
    {"R4", {.id = "R4", .category = DataProbe::Category::Research, .level = 4, .name = "Research Probe G4"}},
    {"R5", {.id = "R5", .category = DataProbe::Category::Research, .level = 5, .name = "Research Probe G5"}},
    {"R6", {.id = "R6", .category = DataProbe::Category::Research, .level = 6, .name = "Research Probe G6"}},
    {"B1", {.id = "B1", .category = DataProbe::Category::Booster, .level = 1, .name = "Booster Probe G1"}},
    {"B2", {.id = "B2", .category = DataProbe::Category::Booster, .level = 2, .name = "Booster Probe G2"}},
    {"S", {.id = "S", .category = DataProbe::Category::Storage, .level = 0, .name = "Storage Probe"}},
    {"D", {.id = "D", .category = DataProbe::Category::Duplicator, .level = 0, .name = "Duplicator Probe"}},
    {"B", {.id = "B", .category = DataProbe::Category::Basic, .level = 0, .name = "Basic Probe"}},
    {"CF", {.id = "CF", .category = DataProbe::Category::Battle, .level = 0, .name = "Fuel Recovery Probe"}},
    {"CM", {.id = "CM", .category = DataProbe::Category::Battle, .level = 0, .name = "Melee Attack Probe"}},
    {"CR", {.id = "CR", .category = DataProbe::Category::Battle, .level = 0, .name = "Ranged Attack Probe"}},
    {"CD", {.id = "CD", .category = DataProbe::Category::Battle, .level = 0, .name = "EZ Debuff Probe"}},
    {"CA", {.id = "CA", .category = DataProbe::Category::Battle, .level = 0, .name = "Attribute Resistance Probe"}},
};
