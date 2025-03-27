//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_PROBE_ARRANGEMENT_H
#define XENOPROBES_PROBE_ARRANGEMENT_H

#include "probe.h"
#include "site.h"

#include <atomic>
#include <random>
#include <vector>

#include <map>

class ProbeArrangement {
public:
  ProbeArrangement() = default;
  ProbeArrangement(const ProbeArrangement &other)
      : probes_(other.probes_), usedProbesDirty_(other.usedProbesDirty_.load()),
        usedProbes_(other.usedProbes_), setupDirty_(other.setupDirty_.load()),
        setup_(other.setup_), storage_weight_(other.storage_weight_),
        production_weight_(other.production_weight_),
        revenue_weight_(other.revenue_weight_) {}
  ProbeArrangement(ProbeArrangement &&other) noexcept
      : probes_(std::move(other.probes_)),
        usedProbesDirty_(other.usedProbesDirty_.load()),
        usedProbes_(std::move(other.usedProbes_)),
        setupDirty_(other.setupDirty_.load()), setup_(std::move(other.setup_)),
        storage_weight_(other.storage_weight_),
        production_weight_(other.production_weight_),
        revenue_weight_(other.revenue_weight_) {}
  ProbeArrangement &operator=(const ProbeArrangement &other) {
    if (this == &other)
      return *this;
    probes_ = other.probes_;
    usedProbesDirty_ = other.usedProbesDirty_.load();
    usedProbes_ = other.usedProbes_;
    setupDirty_ = other.setupDirty_.load();
    setup_ = other.setup_;
    storage_weight_ = other.storage_weight_;
    production_weight_ = other.production_weight_;
    revenue_weight_ = other.revenue_weight_;
    return *this;
  }
  ProbeArrangement &operator=(ProbeArrangement &&other) noexcept {
    if (this == &other)
      return *this;
    probes_ = std::move(other.probes_);
    usedProbesDirty_ = other.usedProbesDirty_.load();
    usedProbes_ = std::move(other.usedProbes_);
    setupDirty_ = other.setupDirty_.load();
    setup_ = std::move(other.setup_);
    storage_weight_ = other.storage_weight_;
    production_weight_ = other.production_weight_;
    revenue_weight_ = other.revenue_weight_;
    return *this;
  }

  void resize(size_t size);
  void setProbeAt(size_t index, const Probe::Ptr &type);
  Probe::Ptr getProbeAt(size_t index) const;

  void mutate(double probability);

  double evaluate() const;

  void randomize();

  void printTotals() const;
  void printSetup() const;
  const std::map<Site::Ptr, Probe::Ptr> &getSetup() const;
  void loadSetup(const std::map<Site::Ptr, Probe::Ptr> &setup);
  const std::map<Probe::Ptr, int> &getUsedProbes() const;

  double getStorageWeight() const;
  void setStorageWeight(double storage_weight);
  double getProductionWeight() const;
  void setProductionWeight(double production_weight);
  double getRevenueWeight() const;
  void setRevenueWeight(double revenue_weight);
  double getTotalProduction() const;
  double getTotalRevenue() const;
  double getTotalStorage() const;
  double getComboBonus(size_t idx) const;
  std::vector<std::string> getOres() const;

  size_t getSize() const;

  friend bool operator==(const ProbeArrangement &l, const ProbeArrangement &r);

  static void seedMT(std::seed_seq &seed);

private:
  int getComboSize(size_t source) const;
  double getProbeBoost(size_t idx) const noexcept;
  double getProbeProduction(size_t idx) const noexcept;
  double getProduction(size_t idx) const noexcept;
  double getProbeRevenue(size_t idx) const noexcept;
  double getRevenue(size_t idx) const noexcept;
  double getStorage(size_t idx) const noexcept;

  std::vector<Probe::Ptr> probes_;

#ifdef __MINGW32__
  static std::mt19937 mt; // note it wasn't seeded properly
#else
  static std::mt19937_64 mt;
#endif

  mutable std::atomic<bool> usedProbesDirty_ = true;
  mutable std::map<Probe::Ptr, int> usedProbes_;
  mutable std::atomic<bool> setupDirty_ = true;
  mutable std::map<Site::Ptr, Probe::Ptr> setup_;
  double storage_weight_ = 1;
  double production_weight_ = 1;
  double revenue_weight_ = 1;
};

#endif // XENOPROBES_PROBE_ARRANGEMENT_H
