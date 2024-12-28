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

  static const Site::Ptr FN101;
  static const Site::Ptr FN102;
  static const Site::Ptr FN103;
  static const Site::Ptr FN104;
  static const Site::Ptr FN105;
  static const Site::Ptr FN106;
  static const Site::Ptr FN107;
  static const Site::Ptr FN108;
  static const Site::Ptr FN109;
  static const Site::Ptr FN110;
  static const Site::Ptr FN111;
  static const Site::Ptr FN112;
  static const Site::Ptr FN113;
  static const Site::Ptr FN114;
  static const Site::Ptr FN115;
  static const Site::Ptr FN116;
  static const Site::Ptr FN117;
  static const Site::Ptr FN118;
  static const Site::Ptr FN119;
  static const Site::Ptr FN120;
  static const Site::Ptr FN121;
  static const Site::Ptr FN201;
  static const Site::Ptr FN202;
  static const Site::Ptr FN203;
  static const Site::Ptr FN204;
  static const Site::Ptr FN205;
  static const Site::Ptr FN206;
  static const Site::Ptr FN207;
  static const Site::Ptr FN208;
  static const Site::Ptr FN209;
  static const Site::Ptr FN210;
  static const Site::Ptr FN211;
  static const Site::Ptr FN212;
  static const Site::Ptr FN213;
  static const Site::Ptr FN214;
  static const Site::Ptr FN215;
  static const Site::Ptr FN216;
  static const Site::Ptr FN217;
  static const Site::Ptr FN218;
  static const Site::Ptr FN219;
  static const Site::Ptr FN220;
  static const Site::Ptr FN221;
  static const Site::Ptr FN222;
  static const Site::Ptr FN223;
  static const Site::Ptr FN224;
  static const Site::Ptr FN225;
  static const Site::Ptr FN301;
  static const Site::Ptr FN302;
  static const Site::Ptr FN303;
  static const Site::Ptr FN304;
  static const Site::Ptr FN305;
  static const Site::Ptr FN306;
  static const Site::Ptr FN307;
  static const Site::Ptr FN308;
  static const Site::Ptr FN309;
  static const Site::Ptr FN310;
  static const Site::Ptr FN311;
  static const Site::Ptr FN312;
  static const Site::Ptr FN313;
  static const Site::Ptr FN314;
  static const Site::Ptr FN315;
  static const Site::Ptr FN316;
  static const Site::Ptr FN317;
  static const Site::Ptr FN318;
  static const Site::Ptr FN319;
  static const Site::Ptr FN320;
  static const Site::Ptr FN321;
  static const Site::Ptr FN322;
  static const Site::Ptr FN401;
  static const Site::Ptr FN402;
  static const Site::Ptr FN403;
  static const Site::Ptr FN404;
  static const Site::Ptr FN405;
  static const Site::Ptr FN406;
  static const Site::Ptr FN407;
  static const Site::Ptr FN408;
  static const Site::Ptr FN409;
  static const Site::Ptr FN410;
  static const Site::Ptr FN411;
  static const Site::Ptr FN412;
  static const Site::Ptr FN413;
  static const Site::Ptr FN414;
  static const Site::Ptr FN415;
  static const Site::Ptr FN416;
  static const Site::Ptr FN417;
  static const Site::Ptr FN418;
  static const Site::Ptr FN419;
  static const Site::Ptr FN420;
  static const Site::Ptr FN501;
  static const Site::Ptr FN502;
  static const Site::Ptr FN503;
  static const Site::Ptr FN504;
  static const Site::Ptr FN505;
  static const Site::Ptr FN506;
  static const Site::Ptr FN507;
  static const Site::Ptr FN508;
  static const Site::Ptr FN509;
  static const Site::Ptr FN510;
  static const Site::Ptr FN511;
  static const Site::Ptr FN512;
  static const Site::Ptr FN513;
  static const Site::Ptr FN514;
  static const Site::Ptr FN515;
  static const Site::Ptr FN516;
  static const std::map<Id, Site::Ptr> ALL;
  static Ptr fromName(Id id);

  Id name;
  Point position{0, 0};
  Grade production = Grade::F;
  Grade revenue = Grade::F;
  Grade combat = Grade::F;
  int sightseeing = 0;

private:
  static const std::unordered_map<Site::Id, std::vector<Site::Ptr>> NEIGHBORS;
};

#endif // XENOPROBES_SITE_H
