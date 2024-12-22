//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_SOLUTION_H
#define XENOPROBES_SOLUTION_H

#include "probe.h"
#include "probe_arrangement.h"
#include <vector>

class Solution {
public:
  Solution();

  int getAge() const;
  void setAge(int);

  double getScore() const;

  void randomize();
  void evaluate();
  void mutate(double rate);

  void printTotals() const;
  void printSetup() const;
  const ProbeArrangement &getSetup() const { return setup_; }
  void setSetup(const ProbeArrangement &setup) { setup_ = setup; }

  bool hasSameArrangement(const Solution &arr2) const;

  Solution findBestChild(size_t numOffsprings, double mutationRate) const;

  bool operator==(const Solution &b);
  bool operator<(const Solution &b);
  bool operator>(const Solution &b);

private:
  double score_ = 0;
  int age_ = 0;
  ProbeArrangement setup_;
};

#endif // XENOPROBES_SOLUTION_H
