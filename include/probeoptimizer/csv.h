/**
 * @file csv.h
 *
 * @author Dan Keenan
 * @date 11/30/24
 * @copyright GNU GPLv3
 */

#ifndef XENOPROBES_CSV_H
#define XENOPROBES_CSV_H

#include <string>
#include <variant>
#include <vector>

using CsvRecordVal = std::variant<std::string, int>;
int csvRecordValToInt(const CsvRecordVal& val);
std::vector<std::vector<CsvRecordVal>> loadCSV(std::string fname);

#endif //XENOPROBES_CSV_H
