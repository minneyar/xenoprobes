//
// Created by preed on 12/27/15.
//

#ifndef XENOPROBES_PROBE_H
#define XENOPROBES_PROBE_H

#include <unordered_map>
#include <vector>
#include <string>

struct Probe {
  friend constexpr std::weak_ordering operator<=>(const Probe &lhs,
                                                  const Probe &rhs) {
    if (lhs.category != rhs.category) {
      return lhs.category <=> rhs.category;
    }
    return lhs.level <=> rhs.level;
  }

  using Id = std::string;
  using Ptr = const Probe *;

  enum class Category {
    Basic,
    Mining,
    Research,
    Booster,
    Storage,
    Duplicator,
    Battle,
  };

  static const Probe::Ptr B;
  static const Probe::Ptr M1;
  static const Probe::Ptr M2;
  static const Probe::Ptr M3;
  static const Probe::Ptr M4;
  static const Probe::Ptr M5;
  static const Probe::Ptr M6;
  static const Probe::Ptr M7;
  static const Probe::Ptr M8;
  static const Probe::Ptr M9;
  static const Probe::Ptr M10;
  static const Probe::Ptr R1;
  static const Probe::Ptr R2;
  static const Probe::Ptr R3;
  static const Probe::Ptr R4;
  static const Probe::Ptr R5;
  static const Probe::Ptr R6;
  static const Probe::Ptr B1;
  static const Probe::Ptr B2;
  static const Probe::Ptr D;
  static const Probe::Ptr S;
  static const Probe::Ptr CF;
  static const Probe::Ptr CM;
  static const Probe::Ptr CR;
  static const Probe::Ptr CD;
  static const Probe::Ptr CA;
  static const std::unordered_map<Id, Probe::Ptr> ALL;
  static const std::vector<Probe::Ptr> ALL_SORTED;
  static Ptr fromString(const std::string &str);

  Id id;
  Category category;
  unsigned int level;
  double production;
  double revenue;
  double boost;
};

#endif // XENOPROBES_PROBE_H
