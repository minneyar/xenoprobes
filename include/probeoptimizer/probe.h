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

  // enum Type {
  //   Basic = 0,
  //   Mining1 = 1,
  //   Mining2 = 2,
  //   Mining3 = 3,
  //   Mining4 = 4,
  //   Mining5 = 5,
  //   Mining6 = 6,
  //   Mining7 = 7,
  //   Mining8 = 8,
  //   Mining9 = 9,
  //   Mining10 = 10,
  //   Research1 = 11,
  //   Research2 = 12,
  //   Research3 = 13,
  //   Research4 = 14,
  //   Research5 = 15,
  //   Research6 = 16,
  //   Booster1 = 17,
  //   Booster2 = 18,
  //   Dupe = 19,
  //   Storage = 20,
  //   Invalid = 21
  // };

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
