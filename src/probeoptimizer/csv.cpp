/**
 * @file csv.cpp
 *
 * @author Dan Keenan
 * @date 11/30/24
 * @copyright GNU GPLv3
 */

#include "probeoptimizer/csv.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <fstream>

std::vector<std::vector<CsvRecordVal>> loadCSV(std::string fname) {
  std::vector<std::vector<CsvRecordVal>> result;

  std::ifstream file{fname};
  if (!file) {
    throw std::runtime_error{"could not open file " + fname};
  }
  std::string line;
  while (getline(file, line)) {
    auto commentPos = line.find('#');
    if (commentPos != std::string::npos)
      line.erase(commentPos);
    boost::trim(line);
    if (line.empty())
      continue;
    std::vector<std::string> records;
    boost::split(records, line, [](char c) { return c == ','; });
    auto &recordVariants = result.emplace_back();
    std::transform(records.begin(), records.end(),
                   std::back_inserter(recordVariants),
                   [](const std::string &s) { return CsvRecordVal(s); });
  }

  return result;
}

int csvRecordValToInt(const CsvRecordVal &val) {
  if (std::holds_alternative<std::string>(val)) {
    return std::stoi(std::get<std::string>(val));
  }
  return std::get<int>(val);
}
