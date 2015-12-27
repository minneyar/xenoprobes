/*
    XenoProbes - a program to optimize probe placement in Xenoblade Chronicles X
    Copyright (C) 2015  Daniel K. O.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <queue>
#include <utility>
#include <cstdio>
#include <cstddef>
#include <algorithm>
#include <random>
#include <utility>
#include <iomanip>
#include <atomic>
#include <csignal>
#include <cassert>

#include <boost/algorithm/string/split.hpp>
#include <boost/program_options.hpp>


#ifdef __MINGW32__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincrypt.h>
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif



using std::size_t;
using std::cout;
using std::clog;
using std::endl;

enum RunMode {
    HelpMode,
    PrintMode,
    OptimizeGAMode,
    OptimizeHCMode
};


float storageWeight;
float revenueWeight;
float productionWeight;

float mutationRate;
float eliteRatio;
size_t maxPopSize;
size_t tournamentRank;
size_t numOffsprings;
size_t maxIterations;

std::string setupInput;
std::string inventoryInput;


#ifdef __MINGW32__
std::mt19937 mt; // note it wasn't seeded properly
#else
std::mt19937_64 mt{std::random_device{}()};
#endif



struct Site {
    int name;
    int production = 0;
    int revenue = 0;
    int sightseeing = 0;
    std::vector<size_t> neighbors;

};



enum ProbeType {
    Basic     = 0,
    Mining1   = 1,
    Mining2   = 2,
    Mining3   = 3,
    Mining4   = 4,
    Mining5   = 5,
    Mining6   = 6,
    Mining7   = 7,
    Mining8   = 8,
    Mining9   = 9,
    Mining10  = 10,
    Research1 = 11,
    Research2 = 12,
    Research3 = 13,
    Research4 = 14,
    Research5 = 15,
    Research6 = 16,
    Booster1  = 17,
    Booster2  = 18,
    Dupe      = 19,
    Storage   = 20,
    Invalid   = 21
};


using ProbeVec = std::vector<ProbeType>;


ProbeType str2ProbeType(const std::string& s)
{
    if (s == "B")
        return Basic;
    if (s[0] == 'M')
        return ProbeType(int(Mining1) + std::stoi(s.substr(1)) - 1);
    if (s[0] == 'R')
        return ProbeType(int(Research1) + std::stoi(s.substr(1)) - 1);
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


std::string probeType2str(ProbeType type)
{
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




struct Probe {
    ProbeType type;
    double production;
    double revenue;
    double boost;

    bool isBasic() const noexcept
    {
        return type == Basic;
    }

    bool isMining() const noexcept
    {
        return type >= Mining1 && type <= Mining10;
    }

    bool isResearch() const noexcept
    {
        return type >= Research1 && type <= Research6;
    }
    
    bool isBooster() const noexcept
    {
        return type >= Booster1 && type <= Booster2;
    }

    bool isDupe() const noexcept
    {
        return type == Dupe;
    }

    bool isStorage() const noexcept
    {
        return type == Storage;
    }
};

std::vector<Probe> probes = {
    {Basic     , 0.5, 0.5, 1.0},
    {Mining1   , 1.0, 0.3, 1.0},
    {Mining2   , 1.2, 0.3, 1.0},
    {Mining3   , 1.4, 0.3, 1.0},
    {Mining4   , 1.6, 0.3, 1.0},
    {Mining5   , 1.8, 0.3, 1.0},
    {Mining6   , 2.0, 0.3, 1.0},
    {Mining7   , 2.2, 0.3, 1.0},
    {Mining8   , 2.4, 0.3, 1.0},
    {Mining9   , 2.7, 0.3, 1.0},
    {Mining10  , 3.0, 0.3, 1.0},
    {Research1 , 0.3, 2.0, 1.0},
    {Research2 , 0.3, 2.5, 1.0},
    {Research3 , 0.3, 3.0, 1.0},
    {Research4 , 0.3, 3.5, 1.0},
    {Research5 , 0.3, 4.0, 1.0},
    {Research6 , 0.3, 4.5, 1.0},
    {Booster1  , 0.1, 0.1, 1.5},
    {Booster2  , 0.1, 0.1, 2.0},
    {Dupe      , 0.0, 0.0, 1.0},
    {Storage   , 0.1, 0.1, 1.0}
};



std::vector<Site> sites;
std::vector<ProbeType> inventory;


std::vector<std::vector<std::string>>
loadCSV(const std::string& fname)
{
    using std::vector;
    using std::string;

    vector<vector<string>> result;

    std::ifstream file{fname};
    if (!file)
        throw std::runtime_error{"could not open file " + fname};
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;
        vector<string> records;
        boost::split(records, line, [](char c) {return c==',';});
        result.push_back(std::move(records));
    }

    return result;
}


size_t getSiteIdx(int name)
{
    for (size_t i=0; i<sites.size(); ++i)
        if (sites[i].name == name)
            return i;
    throw std::exception{};
}


void loadSites()
{
    sites.clear();

    std::map<std::string, int> prodGrade = {
        {"A", 500},
        {"B", 350},
        {"C", 250}
    };
    std::map<std::string, int> revGrade = {
        {"S", 850},
        {"A", 750},
        {"B", 650},
        {"C", 550},
        {"D", 450},
        {"E", 300},
        {"F", 200}
    };


    {
        auto data = loadCSV("sites.csv");
        for (const auto& record : data) {
            Site site;
            site.name        = std::stoi(record[0]);
            site.production  = prodGrade[record[1]];
            site.revenue     = revGrade[record[2]];
            site.sightseeing = std::stoi(record[4]);
            sites.push_back(std::move(site));
        }

        //cout << "nodes: " << sites.size() << endl;
    }
    
    {
        std::ifstream file{"mira.dot"};
        std::string line;
        int count = 0;
        while (getline(file, line)) {
            int u, v;
            if (std::sscanf(line.c_str(), "%d -- %d", &u, &v) == 2) {
                try {
                    size_t uidx = getSiteIdx(u);
                    size_t vidx = getSiteIdx(v);
                    sites[uidx].neighbors.push_back(vidx);
                    sites[vidx].neighbors.push_back(uidx);
                    ++count;
                }
                catch (...) {
                    // ok, site is not accessible yet
                }
            }
        }

        //cout << "edges: " << count << endl;
    }
}





ProbeVec loadSetup(const std::string& name)
{
    ProbeVec setup(sites.size());

    auto data = loadCSV(name);
    for (auto& record : data) {
        int siteName = std::stoi(record[0]);
        setup[getSiteIdx(siteName)] = str2ProbeType(record[1]);
    }
    return setup;
}




int getComboSize(size_t source, const ProbeVec& setup)
{
    const auto stype = setup[source];

    int comboSize = 0;

    static std::vector<char> visited;
    static std::queue<size_t> Q;

    visited.clear();
    visited.resize(setup.size());

    Q.push(source);
    while (!Q.empty()) {
        size_t idx = Q.front();
        Q.pop();

        //clog << "visited " << sites[idx].name << endl;

        visited[idx] = true;
        ++comboSize;

        for (auto nidx : sites[idx].neighbors)
            if (setup[nidx] == stype && !visited[nidx])
                Q.push(nidx);

    }
    return comboSize;
}


double getComboBonus(size_t idx, const ProbeVec& setup)
{
    const auto& probe = probes[setup[idx]];
    if (probe.isDupe() || probe.isBooster())
        return 1.0;

    int sz = getComboSize(idx, setup);
    if (sz < 3)
        return 1.0;

    if (sz < 5)
        return 1.3;
    if (sz < 8)
        return 1.5;
    return 1.8;
}


double getProbeBoost(size_t idx, const ProbeVec& setup) noexcept
{
    const auto& probe = probes[setup[idx]];
    if (!probe.isDupe()) {
        if (probe.isBooster())
            return probe.boost * getComboBonus(idx, setup);
        else
            return probe.boost;
    }

    double boost = 1.0;
    for (auto nidx: sites[idx].neighbors) {
        const auto& neighbor = probes[setup[nidx]];
        boost *= neighbor.boost;
    }
    return boost;
}



double getProbeProduction(size_t idx, const ProbeVec& setup) noexcept
{
    auto type = setup[idx];
    
    const auto& probe = probes[type];
    const auto& site = sites[idx];

    double probeRate = probe.production;

    // check if it's in a combo
    if (probe.isMining())
        probeRate *= getComboBonus(idx, setup);

    double dupeStash = 0; // things that don't get boosted
    if (probe.isDupe()) {
        for (auto nidx : site.neighbors) {
            const auto& neighbor = probes[setup[nidx]];
            if (neighbor.isDupe())
                continue;
            if (neighbor.isMining())
                probeRate += neighbor.production;
            else
                dupeStash += neighbor.production;
        }
    }

    // check if there are neighbor boosters
    if (probe.isMining() || probe.isDupe()) {
        double scale = 1;
        for (auto nidx : site.neighbors) {
            //const auto& neighbor = probes[setup[nidx]];
            double boost = getProbeBoost(nidx, setup);
            scale *= boost;
        }
        //clog << site.name << " mining got boosted by " << scale << endl;
        probeRate *= scale;
    }

    return probeRate + dupeStash;
}


double getProduction(size_t idx, const ProbeVec& setup) noexcept
{
    return getProbeProduction(idx, setup) * sites[idx].production;
}



double getProbeRevenue(size_t idx, const ProbeVec& setup) noexcept
{
    auto type = setup[idx];
    
    const auto& probe = probes[type];
    const auto& site = sites[idx];

    double probeRate = probe.revenue;

    // check if it's in a combo
    if (probe.isResearch())
        probeRate *= getComboBonus(idx, setup);

    double dupeStash = 0; // things that don't get boosted
    if (probe.isDupe()) {
        for (auto nidx : site.neighbors) {
            const auto& neighbor = probes[setup[nidx]];
            if (neighbor.isDupe())
                continue;
            if (neighbor.isResearch())
                probeRate += neighbor.revenue;
            else
                dupeStash += neighbor.revenue;
        }
    }

    // check if there are neighbor boosters
    if (probe.isResearch() || probe.isDupe()) {
        double scale = 1;
        for (auto nidx : site.neighbors) {
            double boost = getProbeBoost(nidx, setup);
            scale *= boost;
        }
        //clog << site.name << " revenue got boosted by " << scale << endl;
        probeRate *= scale;
    }

    return probeRate + dupeStash;
}



double getRevenue(size_t idx, const ProbeVec& setup) noexcept
{
    const auto& site = sites[idx];

    double siteRev = site.revenue;
    if (probes[setup[idx]].isResearch())
        siteRev += 1000*site.sightseeing;

    return getProbeRevenue(idx, setup) * siteRev;
}


double getStorage(size_t idx, const ProbeVec& setup) noexcept
{
    auto type = setup[idx];
    
    const auto& probe = probes[type];
    const auto& site = sites[idx];

    double probeStorage = 0;

    // check if it's in a combo
    if (probe.isStorage()) {
        probeStorage = 3000;
        probeStorage *= getComboBonus(idx, setup);
    }

    if (probe.isDupe()) {
        for (auto nidx : site.neighbors) {
            const auto& neighbor = probes[setup[nidx]];
            if (neighbor.isStorage())
                probeStorage += 3000;
        }
    }

    // check if there are neighbor boosters
    if (probe.isStorage() || probe.isDupe()) {
        double scale = 1;
        for (auto nidx : site.neighbors) {
            //const auto& neighbor = probes[setup[nidx]];
            double boost = getProbeBoost(nidx, setup);
            scale *= boost;
        }
        //clog << site.name << " storage got boosted by " << scale << endl;
        probeStorage *= scale;
    }

    return probeStorage;
}






void loadInventory(const std::string& fileName)
{
    inventory.clear();
    
    auto data = loadCSV(fileName);
    for (const auto& record : data) {
        int num = std::stoi(record[1]);
        for (int i=0; i<num; ++i)
            inventory.push_back(str2ProbeType(record[0]));
    }

    // ensure inventory is as big as needed to fill the entire map
    while (inventory.size() < sites.size())
        inventory.push_back(Basic);
}


void printInventory()
{
    std::map<ProbeType, int> histo;
    for (auto item : inventory)
        ++histo[item];
    for (auto& entry : histo)
        if (entry.first != Basic)
            cout << probeType2str(entry.first) << "," << entry.second << endl;
}



ProbeVec getRandomSetup()
{
    ProbeVec setup = inventory;    
    std::shuffle(setup.begin(), setup.end(), mt);
    return setup;
}


void printSetup(const ProbeVec& setup)
{
    for (size_t idx=0; idx<sites.size(); ++idx) {
        if (setup[idx] != Basic)
            cout << sites[idx].name << "," << probeType2str(setup[idx]) << endl;
    }
}




void mutate(std::vector<ProbeType>& setup, double prob) noexcept
{
    std::bernoulli_distribution dist{prob};

    for (size_t idx=0; idx<setup.size()-1; ++idx) {
        if (dist(mt)) {
            std::uniform_int_distribution<size_t> idist{idx, setup.size()-1};
            size_t dest = idist(mt);
            std::swap(setup[idx], setup[dest]);
        }
    }
}



std::vector<size_t> findOccurrences(const ProbeVec& haystack, ProbeType needle)
{
    std::vector<size_t> result;
    for (size_t i=0; i<haystack.size(); ++i)
        if (haystack[i] == needle)
            result.push_back(i);
    return result;
}



template<class T>
T randomElement(const std::vector<T>& bag)
{
    assert(!bag.empty());
    std::uniform_int_distribution<size_t> dist{0, bag.size()-1};
    return bag[dist(mt)];
}


void dump(const ProbeVec& setup)
{
    for (auto i : setup)
        clog << "." << i;
    clog << endl;
}


/*
 * This is a classic Cycle Crossover operator, as in the textbooks.
 * The advantage is it preservers the absolute positions from the parents.
 */
std::pair<ProbeVec, ProbeVec>
crossover(ProbeVec mom, ProbeVec dad)
{
    const size_t chromoSize = mom.size();

    ProbeVec a(chromoSize, Invalid), b(chromoSize, Invalid);

    std::uniform_int_distribution<size_t> startDist{0, chromoSize-1};
    size_t start = startDist(mt);
    size_t idx = start; 

    do {
        auto momData = mom[idx];
        auto dadData = dad[idx];
        assert(momData != Invalid);
        assert(dadData != Invalid);
        a[idx] = momData;
        b[idx] = dadData;

        if (idx != start) {
            mom[idx] = Invalid;
            dad[idx] = Invalid;
        }

        auto occurrences = findOccurrences(dad, momData);
        if (occurrences.empty()) {
            throw std::logic_error{"something very wrong happened!"};
        }

        idx = randomElement(occurrences);

    } while (idx != start);


    // now complete a and b with the complement
    for (size_t i=0; i<chromoSize; ++i) {
        if (a[i] == Invalid) {
            a[i] = dad[i];
            b[i] = mom[i];
        }
    }
    return {std::move(a), std::move(b)};
}


float evaluate(ProbeVec& setup) noexcept
{
    long totalProd = 0;
    long totalRev = 0;
    long totalStorage = 6000;
    for (size_t i = 0; i<sites.size(); ++i) {
        totalProd += getProduction(i, setup);
        totalRev += getRevenue(i, setup);
        totalStorage += getStorage(i, setup);
    }
    return totalStorage * storageWeight + totalRev * revenueWeight + totalProd * productionWeight;
}


void printTotals(const ProbeVec& setup)
{
    long totalProd = 0;
    long totalRev = 0;
    long totalStorage = 6000;
    for (size_t i = 0; i<sites.size(); ++i) {
        totalProd += getProduction(i, setup);
        totalRev += getRevenue(i, setup);
        totalStorage += getStorage(i, setup);
    }
    cout << "# " << totalProd << " / " << totalRev << " / " << totalStorage << endl;
}


bool areSame(const ProbeVec& a, const ProbeVec& b)
{
    const auto sz = sites.size();
    return std::equal(a.begin(), a.begin()+sz,
                      b.begin());
}


bool isValid(const ProbeVec& setup)
{
    std::map<ProbeType,int> histoInv;
    std::map<ProbeType,int> histoSetup;
    
    for (const auto& item : inventory)
        ++histoInv[item];
    for (const auto& item : setup)
        ++histoSetup[item];
    return histoInv == histoSetup;
}



std::atomic<bool> shouldStop;


void handleSIGINT(int)
{
    shouldStop = true;
}



RunMode parseOptions(int argc, char** argv)
{
    namespace po = boost::program_options;
    
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "prints help message")
        ("print",
         po::value<std::string>(&setupInput),
         "Just print estimated output for a given setup file (in .CSV format.)")
        ("storageweight",
         po::value<float>(&storageWeight)->default_value(1000),
         "How important is Storage (maximum Miranium that can be held at once.)")
        ("revenueweight",
         po::value<float>(&revenueWeight)->default_value(10),
         "How important is Revenue (cash output.)")
        ("productionweight",
         po::value<float>(&productionWeight)->default_value(1),
         "How important is Production (Miranium output.)")
        ("iterations",
         po::value<size_t>(&maxIterations)->default_value(2000),
         "How many iterations to run.")
        ("offsprings",
         po::value<size_t>(&numOffsprings)->default_value(4),
         "(HC ONLY) How many random offsprings to generate from each solution.")
        ("mutation,m",
         po::value<float>(&mutationRate)->default_value(1.0/32),
         "Mutation rate (probability of any probe swapping places with another.)")
        ("inventory,i",
         po::value<std::string>(&inventoryInput)->default_value("inventory.csv"),
         "The inventory file, in .CSV format.")
        ("elite,e",
         po::value<float>(&eliteRatio)->default_value(1.0/128),
         "(GA ONLY) Ratio of best solutions that are copied over to the next generation unchanged.")
        ("population",
         po::value<size_t>(&maxPopSize)->default_value(2000),
         "Maximum population size.")
        ("tournament",
         po::value<size_t>(&tournamentRank)->default_value(10),
         "(GA ONLY) Size of the selection tournament")
        ("ga", "Enable a Genetic Algorithm search (slower and worse) instead of a Hill Climbing search")
        ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return HelpMode;
    }

    if (vm.count("print"))
        return PrintMode;

    if (vm.count("ga"))
        return OptimizeGAMode;

    return OptimizeHCMode;
}


const std::pair<float,ProbeVec>&
doTournament(const std::vector<std::pair<float,ProbeVec>>& population, size_t rank)
{

    std::uniform_int_distribution<size_t> dist{0, population.size()-1};

    std::vector<size_t> competitors;
    size_t bestIdx = dist(mt);
    for (size_t i=1; i<rank; ++i) {
        size_t newIdx = dist(mt);
        if (population[newIdx].first > population[bestIdx].first)
            bestIdx = newIdx;
    }
    return population[bestIdx];
}


void doGeneticAlgorithm()
{
    std::vector<std::pair<float,ProbeVec>> population(maxPopSize), newGen;
    for (size_t i=0; i<maxPopSize; ++i) {
        population[i].second = getRandomSetup();
        population[i].first = evaluate(population[i].second);
    }

    float best = 0;
    float worst = 0;
    std::pair<float,ProbeVec> globalBest{0.0f, {}};

    size_t numElites = std::min(size_t(eliteRatio * maxPopSize), population.size());
    //clog << "Number of elites: " << numElites << endl;

    for (size_t iter=0; iter<maxIterations && !shouldStop; ++iter) {

        clog << "iteration " << iter
             << " best: " << std::setprecision(9) << best
             << " worst: " << std::setprecision(9) << worst
             << " population: " << population.size()
             << endl;

        newGen.clear();

#define TOURNAMENT

#ifndef TOURNAMENT
        // select breeders proportional to fitness
        std::vector<float> probs;
        probs.reserve(population.size());
        for (auto& item : population)
            probs.push_back(item.first);

        std::discrete_distribution<size_t> dist(probs.begin(), probs.end());
#endif


        while (newGen.size()+1+numElites < maxPopSize) {
#ifndef TOURNAMENT
            const ProbeVec& mom = population[dist(mt)].second;
            const ProbeVec& dad = population[dist(mt)].second;
#else
            const ProbeVec& mom = doTournament(population, tournamentRank).second;
            const ProbeVec& dad = doTournament(population, tournamentRank).second;
#endif
            assert(isValid(mom));
            assert(isValid(dad));

            ProbeVec a, b;
            std::tie(a, b) = crossover(mom, dad);

#if 0
            assert(isValid(a));
            assert(isValid(b));
#endif       

            mutate(a, mutationRate);
            mutate(b, mutationRate);

            float aEval = evaluate(a);
            float bEval = evaluate(b);

            newGen.push_back(std::make_pair(aEval, std::move(a)));
            newGen.push_back(std::make_pair(bEval, std::move(b)));
        }

        // bring in the elites
        std::nth_element(population.begin(),
                         population.end()-numElites,
                         population.end());
        newGen.insert(newGen.end(),
                      make_move_iterator(population.end()-numElites),
                      make_move_iterator(population.end()));


#if 0
        std::sort(newGen.rbegin(), newGen.rend());
        auto newGenEnd = std::unique(newGen.begin(), newGen.end(),
                                     [](const std::pair<float,ProbeVec>& a, const std::pair<float,ProbeVec>& b)
                                     {
                                         return a.first==b.first && areSame(a.second, b.second);
                                     });
        clog << "cleaning " << distance(newGenEnd, newGen.end()) << endl;
        newGen.erase(newGenEnd, newGen.end());
#endif




        auto minmax = std::minmax_element(newGen.begin(), newGen.end());
        best = minmax.second->first;
        worst = minmax.first->first;

        if (minmax.second->first > globalBest.first)
            globalBest = *minmax.second;

        swap(population, newGen);
    }


    printSetup(globalBest.second);
    printTotals(globalBest.second);

}




void doHillClimbing()
{
    std::vector<std::pair<float,ProbeVec>> population(maxPopSize), newGen;
    for (size_t i=0; i<maxPopSize; ++i) {
        population[i].second = getRandomSetup();
        population[i].first = evaluate(population[i].second);
    }

    float best = 0;
    float worst = 0;
    std::pair<float,ProbeVec> globalBest{0.0f, {}};

    for (size_t iter=0; iter<maxIterations && !shouldStop; ++iter) {

        clog << "iteration: " << (iter+1) << "/" << maxIterations
             << " best: " << std::setprecision(9) << best
             << " worst: " << std::setprecision(9) << worst
             << " population: " << population.size()
             << endl;

        newGen.clear();

        for (const auto& ancestor : population) {
            for (size_t i=0; i<numOffsprings; ++i) {
                auto child = ancestor.second;
                mutate(child, mutationRate);
                if (areSame(ancestor.second, child))
                    continue;
                float fitness = evaluate(child);
                newGen.push_back(std::make_pair(fitness, std::move(child)));
            }
        }

        newGen.insert(newGen.end(), population.begin(), population.end());
        std::sort(newGen.begin(), newGen.end(),
                  [](const std::pair<float,ProbeVec>& a, const std::pair<float,ProbeVec>& b)
                  {
                      return a.first > b.first;
                  });
        auto newEnd = std::unique(newGen.begin(), newGen.end(),
                                  [](const std::pair<float,ProbeVec>& a, const std::pair<float,ProbeVec>& b)
                                  {
                                      return a.first == b.first && areSame(a.second, b.second);
                                  });
        newGen.erase(newEnd, newGen.end());
        if (newGen.size() > maxPopSize)
            newGen.resize(maxPopSize);

        
        auto minmax = std::minmax_element(newGen.begin(), newGen.end());
        best = minmax.second->first;
        worst = minmax.first->first;

        if (best < worst*1.0005) {
            // kill some and reseed
            clog << "re-seeding" << endl;
            newGen.resize(std::min<size_t>(10, maxPopSize));
            while (newGen.size() < maxPopSize) {
                ProbeVec setup = getRandomSetup();
                float eval = evaluate(setup);
                newGen.push_back(make_pair(eval, std::move(setup)));
            }
        }

        if (minmax.second->first > globalBest.first)
            globalBest = *minmax.second;

        swap(population, newGen);
    }


    printSetup(globalBest.second);
    printTotals(globalBest.second);
}



#ifdef __MINGW32__
void seedRandom()
{
    HCRYPTPROV provider = 0;

    if (!::CryptAcquireContextW(&provider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
        throw std::runtime_error{"could not acquire crypto context"};

    const DWORD length = 256;
    BYTE buffer[length];

    if (!::CryptGenRandom(provider, length, buffer)) {
        ::CryptReleaseContext(provider, 0);
        throw std::runtime_error{"could not generate random numbers!"};
    }

    std::seed_seq seq(buffer, buffer+length);
    mt.seed(seq);

    if (!::CryptReleaseContext(provider, 0))
        throw std::runtime_error{"could not release crypto context"};
}
#endif


int main(int argc, char** argv)
{
#ifdef __MINGW32__
    seedRandom();
#endif

    auto mode = parseOptions(argc, argv);

    if (mode == HelpMode)
        return 0;

    loadSites();

    if (mode == PrintMode) {
        auto setup = loadSetup(setupInput);
        printSetup(setup);
        printTotals(setup);
        return 0;
    }


    loadInventory(inventoryInput);


    std::signal(SIGINT, handleSIGINT);


    if (mode == OptimizeGAMode)
        doGeneticAlgorithm();
    else
        doHillClimbing();
}

