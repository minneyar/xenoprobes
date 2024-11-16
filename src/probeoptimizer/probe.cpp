//
// Created by preed on 12/27/15.
//

#include "probeoptimizer/probe.h"


Probe::Probe(Type type, double production, double revenue, double boost) {
    type_ = type;
    production_ = production;
    revenue_ = revenue;
    boost_ = boost;
}

Probe::Type Probe::fromString(const std::string &s) {
    if (s == "B")
        return Basic;
    if (s[0] == 'M')
        return Type(int(Mining1) + std::stoi(s.substr(1)) - 1);
    if (s[0] == 'R')
        return Type(int(Research1) + std::stoi(s.substr(1)) - 1);
    if (s == "B1")
        return Booster1;
    if (s == "B2")
        return Booster2;
    if (s == "D")
        return Dupe;
    if (s == "S")
        return Storage;
    throw std::exception{};
}

std::string Probe::toString() const {
    return toString(type_);
}

std::string Probe::toString(const Type& type) {
    switch (type) {
        case Basic:
            return "B";
        case Mining1:
        case Mining2:
        case Mining3:
        case Mining4:
        case Mining5:
        case Mining6:
        case Mining7:
        case Mining8:
        case Mining9:
        case Mining10:
            return "M" + std::to_string(type - Mining1 + 1);
        case Research1:
        case Research2:
        case Research3:
        case Research4:
        case Research5:
        case Research6:
            return "R" + std::to_string(type - Research1 + 1);
        case Booster1:
            return "B1";
        case Booster2:
            return "B2";
        case Dupe:
            return "D";
        case Storage:
            return "S";
        default:
            return "invalid";
    }
}

const Probe::Type &Probe::getType() const {
    return type_;
}

void Probe::setType(const Probe::Type &type) {
    type_ = type;
}

double Probe::getProduction() const {
    return production_;
}

void Probe::setProduction(double production) {
    production_ = production;
}

double Probe::getRevenue() const {
    return revenue_;
}

void Probe::setRevenue(double revenue) {
    revenue_ = revenue;
}

double Probe::getBoost() const {
    return boost_;
}

void Probe::setBoost(double boost) {
    boost_ = boost;
}
