//
// Created by preed on 1/6/16.
//

#include "solution.h"
#include "probe_optimizer.h"

Solution::Solution() :
    setup_(ProbeOptimizer::getDefaultArrangement())
{
}

double Solution::getScore() const {
    return score_;
}

void Solution::setScore(double score) {
    score_ = score;
}

int Solution::getAge() const {
    return age_;
}

void Solution::setAge(int age) {
    age_ = age;
}


void Solution::randomize() {
    setup_.randomize();
}

double Solution::evaluate() {
    return setup_.evaluate();
}

void Solution::mutate(double rate) {
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

bool Solution::operator==(const Solution &b) {
    return score_ == b.score_ && setup_ == b.setup_;
}

bool Solution::operator<( const Solution &b) {
    return score_ < b.score_;
}

bool Solution::operator>(const Solution &b) {
    return score_ > b.score_;
}
