/**
 * @file csv.h
 *
 * @author Dan Keenan
 * @date 11/30/24
 * @copyright GNU GPLv3
 */

#ifndef XENOPROBES_CSV_H
#define XENOPROBES_CSV_H

#include <vector>
#include <string>

std::vector<std::vector<std::string>> loadCSV(std::string fname);

#endif //XENOPROBES_CSV_H
