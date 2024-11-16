//
// Created by preed on 1/6/16.
//

#include "probeoptimizer/ore.h"

Ore::Ore(int id, const std::string &name, int importance) :
    id_(id),
    name_(name),
    importance_(importance)
{
}

int Ore::getId() const {
    return id_;
}

std::string Ore::getName() const {
    return name_;
}

int Ore::getImportance() const {
    return importance_;
}
