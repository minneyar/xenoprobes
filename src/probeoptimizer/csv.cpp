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

std::vector<std::vector<std::string>> loadCSV(std::string fname) {
  using std::string;
  using std::vector;

  vector<vector<string>> result;

  std::ifstream file{fname};
  if (!file) {
    throw std::runtime_error{"could not open file " + fname};
  }
  string line;
  while (getline(file, line)) {
    auto commentPos = line.find('#');
    if (commentPos != string::npos)
      line.erase(commentPos);
    boost::trim(line);
    if (line.empty())
      continue;
    vector<string> records;
    boost::split(records, line, [](char c) { return c == ','; });
    result.push_back(std::move(records));
  }

  return result;
}
