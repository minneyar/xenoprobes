//
// Created by preed on 1/6/16.
//

#include "probeoptimizer/solution.h"
#include "probeoptimizer/probe_optimizer.h"

Solution::Solution() :
    setup_(ProbeOptimizer::getDefaultArrangement())
{
}

double Solution::getScore() const {
    return score_;
}

int Solution::getAge() const {
    return age_;
}

void Solution::setAge(const int age) {
    age_ = age;
}


void Solution::randomize() {
    setup_.randomize();
}

void Solution::evaluate() {
    score_ = setup_.evaluate();
}

void Solution::mutate(const double rate) {
    setup_.mutate(rate);
}


void Solution::printTotals() const {
    setup_.printTotals();
}

void Solution::printSetup() const {
    setup_.printSetup();
}

bool Solution::hasSameArrangement(const Solution &arr2) const {
    return setup_ == arr2.setup_;
}


Solution Solution::findBestChild(size_t numOffsprings, double mutationRate) const {
    Solution bestChild = *this;
    for (size_t i=0; i<numOffsprings; ++i) {
        Solution child = *this;
        child.setAge(0);
        child.mutate(mutationRate);

        if (hasSameArrangement(child)) {
            continue;
        }

        child.evaluate();
        if (child > bestChild) {
            bestChild = std::move(child);
        }
    }

    return bestChild;
}

bool Solution::operator==(const Solution &b) const {
    return score_ == b.score_ && setup_ == b.setup_;
}

bool Solution::operator<(const Solution &b) const {
    return score_ < b.score_;
}

bool Solution::operator>(const Solution &b) const {
    return score_ > b.score_;
}
