//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_PROBE_ARRANGEMENT_H
#define XENOPROBES_PROBE_ARRANGEMENT_H

#include <random>
#include <vector>
#include "probe.h"
#include "site_list.h"

class ProbeArrangement {
public:
    void resize(unsigned long size);
    void setProbeAt(int index, const Probe::Type& type);
    Probe::Type getProbeAt(int index) const;
    
    void mutate(double probability);
    
    double evaluate() const;

    void randomize();

    void printTotals() const;
    void printSetup() const;

    double getStorageWeight() const;
    void setStorageWeight(double storage_weight);
    double getProductionWeight() const;
    void setProductionWeight(double production_weight);
    double getRevenueWeight() const;
    void setRevenueWeight(double revenue_weight);

    unsigned long getSize() const;

    friend bool operator==(const ProbeArrangement& l, const ProbeArrangement& r);

private:
    int getComboSize(size_t source) const;
    double getComboBonus(size_t idx) const;
    double getProbeBoost(size_t idx) const noexcept;
    double getProbeProduction(size_t idx) const noexcept;
    double getProduction(size_t idx) const noexcept;
    double getProbeRevenue(size_t idx) const noexcept;
    double getRevenue(size_t idx) const noexcept;
    double getStorage(size_t idx) const noexcept;

    static const std::vector<Probe> PROBE_VALUES;

    std::vector<Probe::Type> probes_;
    static std::mt19937_64 mt;

    double storage_weight_;
    double production_weight_;
    double revenue_weight_;
};


#endif //XENOPROBES_PROBE_ARRANGEMENT_H
