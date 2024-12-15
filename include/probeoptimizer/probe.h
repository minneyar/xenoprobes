//
// Created by preed on 12/27/15.
//

#ifndef XENOPROBES_PROBE_H
#define XENOPROBES_PROBE_H

#include <map>
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

  static const std::map<Id, Probe> ALL;

  static Ptr fromString(const std::string &str);

  Id id;
  Category category;
  unsigned int level;
  double production;
  double revenue;
  double boost;
};

#endif // XENOPROBES_PROBE_H
