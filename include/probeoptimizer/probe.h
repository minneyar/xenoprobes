//
// Created by preed on 12/27/15.
//

#ifndef XENOPROBES_PROBE_H
#define XENOPROBES_PROBE_H

#include <string>

class Probe {
public:
    enum Type {
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

    Probe(Type type, double production, double revenue, double boost);

    bool isBasic() const noexcept
    {
        return type_ == Basic;
    }

    bool isMining() const noexcept
    {
        return type_ >= Mining1 && type_ <= Mining10;
    }

    bool isResearch() const noexcept
    {
        return type_ >= Research1 && type_ <= Research6;
    }
    
    bool isBooster() const noexcept
    {
        return type_ >= Booster1 && type_ <= Booster2;
    }

    bool isDupe() const noexcept
    {
        return type_ == Dupe;
    }

    bool isStorage() const noexcept
    {
        return type_ == Storage;
    }

    static Type fromString(const std::string& str);
    std::string toString() const;
    static std::string toString(const Type& type);


    const Type &getType() const;
    void setType(const Type &type);
    double getProduction() const;
    void setProduction(double production);
    double getRevenue() const;
    void setRevenue(double revenue);
    double getBoost() const;
    void setBoost(double boost);

private:
    Type type_;
    double production_;
    double revenue_;
    double boost_;

};


#endif //XENOPROBES_PROBE_H
