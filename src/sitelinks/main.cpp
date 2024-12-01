/**
 * @file main.cpp
 *
 * @author Dan Keenan
 * @date 11/30/24
 * @copyright GNU GPLv3
 */

#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: sitelinks <mira.dot> <out.cpp>" << std::endl;
    return 1;
  }

  // Open I/O.
  std::ifstream infile(argv[1]);
  if (!infile.is_open()) {
    std::cerr << "Failed to open file " << argv[1] << std::endl;
    return 1;
  }
  std::ofstream outfile(argv[2]);
  if (!outfile.is_open()) {
    std::cerr << "Unable to open file " << argv[2] << std::endl;
    return 1;
  }

  // Write header.
  outfile
      << "/* THIS FILE IS AUTOMATICALLY GENERATED */\n"
         "\n"
         "#include \"probeoptimizer/site_links.h\"\n"
         "\n"
         "const std::vector<std::array<unsigned int, 2>> kAllSiteLinks {\n";

  std::string line;
  while (std::getline(infile, line)) {
    int u, v;
    if (std::sscanf(line.c_str(), "%d -- %d", &u, &v) == 2) {
      outfile << "  {" << u << " ," << v << "},\n";
    }
  }
  outfile << "};\n";

  return 0;
}
