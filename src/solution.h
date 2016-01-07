//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_SOLUTION_H
#define XENOPROBES_SOLUTION_H

#include <vector>
#include "probe.h"
#include "probe_arrangement.h"

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

    bool hasSameArrangement(const Solution& arr2) const;

    Solution findBestChild(size_t numOffsprings, double mutationRate) const;

    bool operator==(const Solution& b);
    bool operator<(const Solution& b);
    bool operator>(const Solution& b);

private:
    double score_ = 0;
    int age_ = 0;
    ProbeArrangement setup_;
};


#endif //XENOPROBES_SOLUTION_H
