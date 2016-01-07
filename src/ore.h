//
// Created by preed on 1/6/16.
//

#ifndef XENOPROBES_ORE_H
#define XENOPROBES_ORE_H

#include <string>

class Ore {
public:
    Ore(int id, const std::string& name, int importance);

    int getId() const;
    std::string getName() const;
    int getImportance() const;

private:
    int id_;
    std::string name_;
    int importance_;
};


#endif //XENOPROBES_ORE_H
