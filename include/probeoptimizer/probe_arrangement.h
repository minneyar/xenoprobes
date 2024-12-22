//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_PROBE_ARRANGEMENT_H
#define XENOPROBES_PROBE_ARRANGEMENT_H

#include "probe.h"
#include "site.h"
#include <random>
#include <vector>

#include <map>

class ProbeArrangement {
public:
    void resize(size_t size);
    void setProbeAt(size_t index, const Probe::Ptr& type);
    Probe::Ptr getProbeAt(size_t index) const;
    
    void mutate(double probability);
    
    double evaluate() const;

    void randomize();

    void printTotals() const;
    void printSetup() const;
    std::map<Site::Ptr, Probe::Ptr> getSetup() const;

    double getStorageWeight() const;
    void setStorageWeight(double storage_weight);
    double getProductionWeight() const;
    void setProductionWeight(double production_weight);
    double getRevenueWeight() const;
    void setRevenueWeight(double revenue_weight);
    double getTotalProduction() const;
    double getTotalRevenue() const;
    double getTotalStorage() const;
    std::vector<std::string> getOres() const;

    size_t getSize() const;

    friend bool operator==(const ProbeArrangement& l, const ProbeArrangement& r);

    static void seedMT(std::seed_seq& seed);

private:
    int getComboSize(size_t source) const;
    double getComboBonus(size_t idx) const;
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

    double storage_weight_;
    double production_weight_;
    double revenue_weight_;
};


#endif //XENOPROBES_PROBE_ARRANGEMENT_H
