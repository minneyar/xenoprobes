//
// Created by preed on 1/6/16.
//

#include <queue>
#include <set>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "probe_arrangement.h"
#include "probe_optimizer.h"

#ifdef __MINGW32__
std::mt19937 ProbeArrangement::mt; // note it wasn't seeded properly
#else
std::mt19937_64 ProbeArrangement::mt{std::random_device{}()};
#endif

const std::vector<Probe> ProbeArrangement::PROBE_VALUES = {
        Probe(Probe::Type::Basic     , 0.5, 0.5, 1.0),
        Probe(Probe::Type::Mining1   , 1.0, 0.3, 1.0),
        Probe(Probe::Type::Mining2   , 1.2, 0.3, 1.0),
        Probe(Probe::Type::Mining3   , 1.4, 0.3, 1.0),
        Probe(Probe::Type::Mining4   , 1.6, 0.3, 1.0),
        Probe(Probe::Type::Mining5   , 1.8, 0.3, 1.0),
        Probe(Probe::Type::Mining6   , 2.0, 0.3, 1.0),
        Probe(Probe::Type::Mining7   , 2.2, 0.3, 1.0),
        Probe(Probe::Type::Mining8   , 2.4, 0.3, 1.0),
        Probe(Probe::Type::Mining9   , 2.7, 0.3, 1.0),
        Probe(Probe::Type::Mining10  , 3.0, 0.3, 1.0),
        Probe(Probe::Type::Research1 , 0.3, 2.0, 1.0),
        Probe(Probe::Type::Research2 , 0.3, 2.5, 1.0),
        Probe(Probe::Type::Research3 , 0.3, 3.0, 1.0),
        Probe(Probe::Type::Research4 , 0.3, 3.5, 1.0),
        Probe(Probe::Type::Research5 , 0.3, 4.0, 1.0),
        Probe(Probe::Type::Research6 , 0.3, 4.5, 1.0),
        Probe(Probe::Type::Booster1  , 0.1, 0.1, 1.5),
        Probe(Probe::Type::Booster2  , 0.1, 0.1, 2.0),
        Probe(Probe::Type::Dupe      , 0.0, 0.0, 1.0),
        Probe(Probe::Type::Storage   , 0.1, 0.1, 1.0)
};

void ProbeArrangement::resize(size_t size) {
    std::cout << "Resizing to " << size << std::endl;
    probes_.resize(size);
}

void ProbeArrangement::setProbeAt(size_t index, const Probe::Type &type) {
    std::cout << "Setting probe at " << index << " to " << type << std::endl;
    probes_[index] = type;
}


bool operator==(const ProbeArrangement &l, const ProbeArrangement &r) {
    if (l.probes_.size() != r.probes_.size()) {
        return false;
    }
    for (int i = 0; i < l.probes_.size(); i++) {
        if (l.probes_[i] != r.probes_[i]) {
            return false;
        }
    }
    return true;
}

void ProbeArrangement::seedMT(std::seed_seq& seed) {
    mt.seed(seed);
}

Probe::Type ProbeArrangement::getProbeAt(size_t index) const {
    return Probe::Type::Mining3;
}

void ProbeArrangement::mutate(double probability) {
    std::bernoulli_distribution dist{probability};
    // any swap is equally likely, but we don't care about extra probes in the inventory
    for (size_t idx=0; idx<ProbeOptimizer::getSites().size()-1; ++idx) {
        if (dist(mt)) {
            std::uniform_int_distribution<size_t> idist{idx, probes_.size()-1};
            size_t dest = idist(mt);
            std::swap(probes_[idx], probes_[dest]);
        }
    }

}

double ProbeArrangement::evaluate() const {
    double totalProd = 0;
    double totalRev = 0;
    double totalStorage = 6000;
    for (size_t i = 0; i < ProbeOptimizer::getSites().size(); ++i) {
        totalProd += getProduction(i);
        totalRev += getRevenue(i);
        totalStorage += getStorage(i);
    }

    return totalStorage * storage_weight_
           + totalRev * revenue_weight_
           + totalProd * production_weight_;
}

void ProbeArrangement::randomize() {
    probes_ = ProbeOptimizer::getInventory();
    std::shuffle(probes_.begin(), probes_.end(), mt);
}

void ProbeArrangement::printSetup() const {
    int prevName = 100;
    for (size_t idx=0; idx<ProbeOptimizer::getSites().size(); ++idx) {
        if (probes_[idx] != Probe::Type::Basic) {
            if (ProbeOptimizer::getSites().getSite(idx).getName() / 100 != prevName / 100) {
                std::cout << std::endl;
            }
            std::cout << ProbeOptimizer::getSites().getSite(idx).getName() << "," << Probe::toString(probes_[idx]) << std::endl;
            prevName = ProbeOptimizer::getSites().getSite(idx).getName();
        }
    }
}


double ProbeArrangement::getStorageWeight() const {
    return storage_weight_;
}

double ProbeArrangement::getProductionWeight() const {
    return production_weight_;
}

double ProbeArrangement::getRevenueWeight() const {
    return revenue_weight_;
}

void ProbeArrangement::setStorageWeight(double storage_weight) {
    storage_weight_ = storage_weight;
}

void ProbeArrangement::setProductionWeight(double production_weight) {
    production_weight_ = production_weight;
}

void ProbeArrangement::setRevenueWeight(double revenue_weight) {
    revenue_weight_ = revenue_weight;
}

size_t ProbeArrangement::getSize() const {
    return probes_.size();
}

void ProbeArrangement::printTotals() const {
    double totalProd = 0;
    double totalRev = 0;
    double totalStorage = 6000;
    // keep track of which ores have a mining probe on them
    // TODO: figure out how different probes affect ore extraction
    std::set<int> minedOres;
    for (size_t i = 0; i<ProbeOptimizer::getSites().size(); ++i) {
        totalProd += getProduction(i);
        totalRev += getRevenue(i);
        totalStorage += getStorage(i);

        const Probe& probe = PROBE_VALUES[probes_[i]];
        if (probe.isMining()) {
            for (auto& oreId : ProbeOptimizer::getSites().getSite(i).getOres())
                minedOres.insert(oreId);
        }
    }
    std::cout << "# Miranium: " << std::setw(6) << totalProd << "\n"
    << "# Revenue:  " << std::setw(6) << totalRev << "\n"
    << "# Storage:  " << std::setw(6) << totalStorage << "\n"
    << "# Ores (" << minedOres.size() << "/" << ProbeOptimizer::getSites().getOreCount() << "):\n";
    std::vector<std::string> oreNames;
    for (const auto& oreId : minedOres)
        oreNames.push_back(ProbeOptimizer::getSites().getOreByIndex(oreId).getName());
    std::sort(oreNames.begin(), oreNames.end());
    for (const auto& name : oreNames)
        std::cout << "#  " << name << std::endl;
}

int ProbeArrangement::getComboSize(size_t source) const
{
    const auto stype = probes_[source];

    int comboSize = 0;

    // TODO Why were these static...?
    std::vector<char> visited;
    std::queue<size_t> Q;

    visited.clear();
    visited.resize(probes_.size());

    Q.push(source);
    while (!Q.empty()) {
        size_t idx = Q.front();
        Q.pop();

        visited[idx] = true;
        ++comboSize;

        for (auto nidx : ProbeOptimizer::getSites().getSite(idx).getNeighbors())
        {
            if (probes_[nidx] == stype && !visited[nidx])
            {
                Q.push(nidx);
            }
        }

    }
    return comboSize;
}


double ProbeArrangement::getComboBonus(size_t idx) const
{
    const auto& probe = PROBE_VALUES[probes_[idx]];
    if (probe.isDupe() || probe.isBooster())
        return 1.0;

    int sz = getComboSize(idx);
    if (sz < 3)
        return 1.0;

    if (sz < 5)
        return 1.3;
    if (sz < 8)
        return 1.5;
    return 1.8;
}


double ProbeArrangement::getProbeBoost(size_t idx) const noexcept
{
    const auto& probe = PROBE_VALUES[probes_[idx]];
    if (!probe.isDupe()) {
        if (probe.isBooster())
            return probe.getBoost() * getComboBonus(idx);
        else
            return probe.getBoost();
    }

    double boost = 1.0;
    for (auto nidx: ProbeOptimizer::getSites().getSite(idx).getNeighbors()) {
        const auto& neighbor = PROBE_VALUES[probes_[nidx]];
        boost *= neighbor.getBoost();
    }
    return boost;
}



double ProbeArrangement::getProbeProduction(size_t idx) const noexcept
{
    auto type = probes_[idx];

    const auto& probe = PROBE_VALUES[type];
    const auto& site = ProbeOptimizer::getSites().getSite(idx);

    double probeRate = probe.getProduction();

    // check if it's in a combo
    if (probe.isMining())
        probeRate *= getComboBonus(idx);

    double dupeStash = 0; // things that don't get boosted
    if (probe.isDupe()) {
        for (auto nidx : site.getNeighbors()) {
            const auto& neighbor = PROBE_VALUES[probes_[nidx]];
            if (neighbor.isDupe())
                continue;
            if (neighbor.isMining())
                probeRate += neighbor.getProduction();
            else
                dupeStash += neighbor.getProduction();
        }
    }

    // check if there are neighbor boosters
    if (probe.isMining() || probe.isDupe()) {
        double scale = 1;
        for (auto nidx : site.getNeighbors()) {
            //const auto& neighbor = probes[setup[nidx]];
            double boost = getProbeBoost(nidx);
            scale *= boost;
        }
        //clog << site.name << " mining got boosted by " << scale << endl;
        probeRate *= scale;
    }

    return probeRate + dupeStash;
}


double ProbeArrangement::getProduction(size_t idx) const noexcept
{
    return getProbeProduction(idx) * ProbeOptimizer::getSites().getSite(idx).getProduction();
}



double ProbeArrangement::getProbeRevenue(size_t idx) const noexcept
{
    auto type = probes_[idx];

    const auto& probe = PROBE_VALUES[type];
    const auto& site = ProbeOptimizer::getSites().getSite(idx);

    double probeRate = probe.getRevenue();

    // check if it's in a combo
    if (probe.isResearch())
        probeRate *= getComboBonus(idx);

    double dupeStash = 0; // things that don't get boosted
    if (probe.isDupe()) {
        for (auto nidx : site.getNeighbors()) {
            const auto& neighbor = PROBE_VALUES[probes_[nidx]];
            if (neighbor.isDupe())
                continue;
            if (neighbor.isResearch())
                probeRate += neighbor.getRevenue();
            else
                dupeStash += neighbor.getRevenue();
        }
    }

    // check if there are neighbor boosters
    if (probe.isResearch() || probe.isDupe()) {
        double scale = 1;
        for (auto nidx : site.getNeighbors()) {
            double boost = getProbeBoost(nidx);
            scale *= boost;
        }
        //clog << site.name << " revenue got boosted by " << scale << endl;
        probeRate *= scale;
    }

    return probeRate + dupeStash;
}



double ProbeArrangement::getRevenue(size_t idx) const noexcept
{
    const auto& site = ProbeOptimizer::getSites().getSite(idx);

    double siteRev = site.getRevenue();
    if (PROBE_VALUES[probes_[idx]].isResearch())
        siteRev += 1000*site.getSightseeing();

    return getProbeRevenue(idx) * siteRev;
}


double ProbeArrangement::getStorage(size_t idx) const noexcept
{
    auto type = probes_[idx];

    const auto& probe = PROBE_VALUES[type];
    const auto& site = ProbeOptimizer::getSites().getSite(idx);

    double probeStorage = 0;

    // check if it's in a combo
    if (probe.isStorage()) {
        probeStorage = 3000;
        probeStorage *= getComboBonus(idx);
    }

    if (probe.isDupe()) {
        for (auto nidx : site.getNeighbors()) {
            const auto& neighbor = PROBE_VALUES[probes_[nidx]];
            if (neighbor.isStorage())
                probeStorage += 3000;
        }
    }

    // check if there are neighbor boosters
    if (probe.isStorage() || probe.isDupe()) {
        double scale = 1;
        for (auto nidx : site.getNeighbors()) {
            //const auto& neighbor = probes[setup[nidx]];
            double boost = getProbeBoost(nidx);
            scale *= boost;
        }
        //clog << site.name << " storage got boosted by " << scale << endl;
        probeStorage *= scale;
    }

    return probeStorage;
}



