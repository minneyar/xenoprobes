//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_SITE_H
#define XENOPROBES_SITE_H

#include "ore.h"
#include <map>
#include <unordered_map>
#include <vector>

struct Site {
  friend auto operator<=>(const Site &, const Site &) = default;

  using Ptr = const Site *;
  using Id = int;
  using Point = std::pair<int, int>;

  enum class Grade { F, E, D, C, B, A, S };
  static Grade gradeFromChar(char grade);
  static char gradeToChar(Grade grade);

  int getProductionVal() const;
  int getRevenueVal() const;
  std::vector<Ptr> getNeighbors() const;
  const std::vector<Ore::Ptr> &getOre() const;

  static const std::map<Id, Site> ALL;
  static Ptr fromName(Id id);

  Id name;
  Point position{0, 0};
  Grade production = Grade::F;
  Grade revenue = Grade::F;
  Grade combat = Grade::F;
  int sightseeing = 0;
};

#endif // XENOPROBES_SITE_H
