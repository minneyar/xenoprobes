//
// Created by preed on 1/6/16.
//

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>
#include <set>

#include "probeoptimizer/probe_arrangement.h"
#include "probeoptimizer/probe_optimizer.h"

#include <fmt/ranges.h>
#include <spdlog/spdlog.h>

#ifdef __MINGW32__
std::mt19937 ProbeArrangement::mt; // note it wasn't seeded properly
#else
std::mt19937_64 ProbeArrangement::mt{std::random_device{}()};
#endif

void ProbeArrangement::resize(size_t size) {
  spdlog::debug("Resizing to {}", size);
  probes_.resize(size);
}

void ProbeArrangement::setProbeAt(size_t index, const Probe::Ptr &type) {
  spdlog::debug("Setting probe at {} to {}", index, type->id);
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

void ProbeArrangement::seedMT(std::seed_seq &seed) { mt.seed(seed); }

Probe::Ptr ProbeArrangement::getProbeAt(size_t index) const {
  return probes_.at(index);
}

void ProbeArrangement::mutate(double probability) {
  std::bernoulli_distribution dist{probability};
  // any swap is equally likely, but we don't care about extra probes in the
  // inventory
  for (size_t idx = 0; idx < ProbeOptimizer::getSites().size() - 1; ++idx) {
    if (dist(mt)) {
      std::uniform_int_distribution<size_t> idist{idx, probes_.size() - 1};
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

  return totalStorage * storage_weight_ + totalRev * revenue_weight_ +
         totalProd * production_weight_;
}

void ProbeArrangement::randomize() {
  probes_ = ProbeOptimizer::getInventory();
  std::shuffle(probes_.begin(), probes_.end(), mt);
}

void ProbeArrangement::printSetup() const {
  for (const auto [site, probe] : getSetup()) {
    if (probe->category != Probe::Category::Basic) {
      spdlog::info("{},{}", site, probe->id);
    }
  }
}

std::map<int, Probe::Ptr> ProbeArrangement::getSetup() const {
  std::map<int, Probe::Ptr> setup;
  for (size_t idx = 0; idx < ProbeOptimizer::getSites().size(); ++idx) {
    setup.emplace(ProbeOptimizer::getSites().at(idx)->name, probes_[idx]);
  }

  return setup;
}

double ProbeArrangement::getStorageWeight() const { return storage_weight_; }

double ProbeArrangement::getProductionWeight() const {
  return production_weight_;
}

double ProbeArrangement::getRevenueWeight() const { return revenue_weight_; }

void ProbeArrangement::setStorageWeight(double storage_weight) {
  storage_weight_ = storage_weight;
}

void ProbeArrangement::setProductionWeight(double production_weight) {
  production_weight_ = production_weight;
}

void ProbeArrangement::setRevenueWeight(double revenue_weight) {
  revenue_weight_ = revenue_weight;
}

size_t ProbeArrangement::getSize() const { return probes_.size(); }

void ProbeArrangement::printTotals() const {
  const auto totalProd = getTotalProduction();
  const auto totalRev = getTotalRevenue();
  const auto totalStorage = getTotalStorage();
  const auto minedOres = getOres();
  spdlog::info("# Miranium: {totalProd: 6}\n"
               "# Revenue:  {totalRev: 6}\n"
               "# Storage:  {totalStorage: 6}\n"
               "# Ores ({minedOresSize}/{oreCount}):\n"
               "#  {oreNames}",
               fmt::arg("totalProd", totalProd), fmt::arg("totalRev", totalRev),
               fmt::arg("totalStorage", totalStorage),
               fmt::arg("minedOresSize", minedOres.size()),
               fmt::arg("oreCount", Ore::ALL.size()),
               fmt::arg("oreNames", fmt::join(minedOres, "\n#  ")));
}

double ProbeArrangement::getTotalProduction() const {
  double total = 0;
  for (size_t i = 0; i < ProbeOptimizer::getSites().size(); ++i) {
    total += getProduction(i);
  }
  return total;
}

double ProbeArrangement::getTotalRevenue() const {
  double total = 0;
  for (size_t i = 0; i < ProbeOptimizer::getSites().size(); ++i) {
    total += getRevenue(i);
  }
  return total;
}

double ProbeArrangement::getTotalStorage() const {
  double total = 0;
  for (size_t i = 0; i < ProbeOptimizer::getSites().size(); ++i) {
    total += getStorage(i);
  }
  return total;
}

std::vector<std::string> ProbeArrangement::getOres() const {
  // TODO: figure out how different probes affect ore extraction
  std::set<Ore::Ptr> minedOres;
  for (size_t i = 0; i < ProbeOptimizer::getSites().size(); ++i) {
    const auto probe = probes_[i];
    if (probe->category == Probe::Category::Mining) {
      const auto &siteOre = ProbeOptimizer::getSites().at(i)->getOre();
      minedOres.insert(siteOre.cbegin(), siteOre.cend());
    }
  }
  std::vector<std::string> oreNames;
  for (const auto ore : minedOres)
    oreNames.push_back(ore->name);
  std::sort(oreNames.begin(), oreNames.end());
  return oreNames;
}

int ProbeArrangement::getComboSize(size_t source) const {
  const auto sourceType = probes_[source];

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

    for (auto neighbor : ProbeOptimizer::getSites().at(idx)->getNeighbors()) {
      const auto nidx = ProbeOptimizer::getIndexForSiteId(neighbor->name);
      if (probes_[nidx] == sourceType && !visited[nidx]) {
        Q.push(nidx);
      }
    }
  }
  return comboSize;
}

double ProbeArrangement::getComboBonus(size_t idx) const {
  const auto &probe = probes_[idx];
  if (probe->category == Probe::Category::Booster ||
      probe->category == Probe::Category::Duplicator)
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

double ProbeArrangement::getProbeBoost(size_t idx) const noexcept {
  const auto probe = probes_[idx];
  if (probe->category != Probe::Category::Duplicator) {
    if (probe->category == Probe::Category::Booster)
      return probe->boost * getComboBonus(idx);
    else
      return probe->boost;
  }

  double boost = 1.0;
  for (auto neighbor : ProbeOptimizer::getSites().at(idx)->getNeighbors()) {
    const auto nidx = ProbeOptimizer::getIndexForSiteId(neighbor->name);
    const auto neighborProbe = probes_[nidx];
    boost *= neighborProbe->boost;
  }
  return boost;
}

double ProbeArrangement::getProbeProduction(size_t idx) const noexcept {
  const auto probe = probes_[idx];
  const auto site = ProbeOptimizer::getSites().at(idx);

  double probeRate = probe->production;

  // check if it's in a combo
  if (probe->category == Probe::Category::Mining)
    probeRate *= getComboBonus(idx);

  double dupeStash = 0; // things that don't get boosted
  if (probe->category == Probe::Category::Duplicator) {
    for (auto neighbor : site->getNeighbors()) {
      const auto nidx = ProbeOptimizer::getIndexForSiteId(neighbor->name);
      const auto neighborProbe = probes_[nidx];
      if (neighborProbe->category == Probe::Category::Duplicator)
        continue;
      if (neighborProbe->category == Probe::Category::Mining)
        probeRate += neighborProbe->production;
      else
        dupeStash += neighborProbe->production;
    }
  }

  // check if there are neighbor boosters
  if (probe->category == Probe::Category::Mining ||
      probe->category == Probe::Category::Duplicator) {
    double scale = 1;
    for (auto neighbor : site->getNeighbors()) {
      const auto nidx = ProbeOptimizer::getIndexForSiteId(neighbor->name);
      double boost = getProbeBoost(nidx);
      scale *= boost;
    }
    // clog << site.name << " mining got boosted by " << scale << endl;
    probeRate *= scale;
  }

  return probeRate + dupeStash;
}

double ProbeArrangement::getProduction(size_t idx) const noexcept {
  return getProbeProduction(idx) *
         ProbeOptimizer::getSites().at(idx)->getProductionVal();
}

double ProbeArrangement::getProbeRevenue(size_t idx) const noexcept {
  const auto probe = probes_[idx];
  const auto site = ProbeOptimizer::getSites().at(idx);

  double probeRate = probe->revenue;

  // check if it's in a combo
  if (probe->category == Probe::Category::Research)
    probeRate *= getComboBonus(idx);

  double dupeStash = 0; // things that don't get boosted
  if (probe->category == Probe::Category::Duplicator) {
    for (auto neighbor : site->getNeighbors()) {
      const auto nidx = ProbeOptimizer::getIndexForSiteId(neighbor->name);
      const auto neighborProbe = probes_[nidx];
      if (neighborProbe->category == Probe::Category::Duplicator)
        continue;
      if (neighborProbe->category == Probe::Category::Research)
        probeRate += neighborProbe->revenue;
      else
        dupeStash += neighborProbe->revenue;
    }
  }

  // check if there are neighbor boosters
  if (probe->category == Probe::Category::Research ||
      probe->category == Probe::Category::Duplicator) {
    double scale = 1;
    for (auto neighbor : site->getNeighbors()) {
      const auto nidx = ProbeOptimizer::getIndexForSiteId(neighbor->name);
      double boost = getProbeBoost(nidx);
      scale *= boost;
    }
    // clog << site.name << " revenue got boosted by " << scale << endl;
    probeRate *= scale;
  }

  return probeRate + dupeStash;
}

double ProbeArrangement::getRevenue(size_t idx) const noexcept {
  const auto site = ProbeOptimizer::getSites().at(idx);

  double siteRev = site->getRevenueVal();
  if (probes_[idx]->category == Probe::Category::Research)
    siteRev += 1000 * site->sightseeing;

  return getProbeRevenue(idx) * siteRev;
}

double ProbeArrangement::getStorage(size_t idx) const noexcept {
  const auto probe = probes_[idx];
  const auto site = ProbeOptimizer::getSites().at(idx);

  double probeStorage = 0;

  // check if it's in a combo
  if (probe->category == Probe::Category::Storage) {
    probeStorage = 3000;
    probeStorage *= getComboBonus(idx);
  }

  if (probe->category == Probe::Category::Duplicator) {
    for (auto neighbor : site->getNeighbors()) {
      const auto nidx = ProbeOptimizer::getIndexForSiteId(neighbor->name);
      const auto neighborProbe = probes_[nidx];
      if (neighborProbe->category == Probe::Category::Storage)
        probeStorage += 3000;
    }
  }

  // check if there are neighbor boosters
  if (probe->category == Probe::Category::Storage ||
      probe->category == Probe::Category::Duplicator) {
    double scale = 1;
    for (auto neighbor : site->getNeighbors()) {
      const auto nidx = ProbeOptimizer::getIndexForSiteId(neighbor->name);
      double boost = getProbeBoost(nidx);
      scale *= boost;
    }
    // clog << site.name << " storage got boosted by " << scale << endl;
    probeStorage *= scale;
  }

  return probeStorage;
}
