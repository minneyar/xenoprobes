//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_SITE_H
#define XENOPROBES_SITE_H

#include <cstddef>
#include <vector>

class Site {
public:
  using Point = std::pair<int, int>;
  enum class Grade { F, E, D, C, B, A, S };
  static Grade gradeFromChar(char grade);

  explicit Site(int name) : name_(name) {};

  int getName() const;
  Point getPosition() const;
  void setPosition(const Point &position);
  Grade getProduction() const;
  int getProductionVal() const;
  void setProduction(Grade grade);
  Grade getRevenue() const;
  int getRevenueVal() const;
  void setRevenue(Grade grade);
  Grade getCombat() const;
  void setCombat(Grade grade);
  int getSightseeing() const;
  void setSightseeing(int sightseeing);

  void addNeighbor(std::size_t neighborId);
  std::vector<std::size_t> getNeighbors() const;
  void addOre(int oreId);
  std::vector<int> getOres() const;

private:
  int name_;
  Point position_{0, 0};
  Grade production_ = Grade::F;
  Grade revenue_ = Grade::F;
  Grade combat_ = Grade::F;
  int sightseeing_ = 0;
  std::vector<std::size_t> neighbors_;
  std::vector<int> ores_;
};

#endif // XENOPROBES_SITE_H
