/*
    XenoProbes - a program to optimize probe placement in Xenoblade Chronicles X
    Copyright (C) 2015  Daniel K. O.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "probeoptimizer/probe_optimizer.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <csignal>
#include <iostream>
#include <spdlog/spdlog.h>

#ifdef __MINGW32__
#define WIN32_LEAN_AND_MEAN
#include <wincrypt.h>
#include <windows.h>
#endif

namespace po = boost::program_options;

using std::cerr;
using std::clog;
using std::cout;
using std::endl;
using std::size_t;

enum RunMode { HelpMode, PrintMode, OptimizeMode };

#ifdef __MINGW32__
std::mt19937 mt; // note it wasn't seeded properly
#else
std::mt19937_64 mt{std::random_device{}()};
#endif

po::variables_map parseOptions(int argc, const char **argv,
                               ProbeOptimizer &optimizer) {

  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "prints help message")(
      "print",
      "Just print estimated output for a given setup file (in CSV format.)")(
      "storageweight", po::value<float>()->default_value(1000),
      "How important is Storage (maximum Miranium that can be held at once.)")(
      "revenueweight", po::value<float>()->default_value(10),
      "How important is Revenue (cash output.)")(
      "productionweight", po::value<float>()->default_value(1),
      "How important is Production (Miranium output.)")(
      "iterations", po::value<size_t>()->default_value(2000),
      "How many iterations to run.")(
      "offsprings,o", po::value<size_t>()->default_value(100),
      "How many random variations to generate from each solution.")(
      "mutation,m", po::value<float>()->default_value(1.0 / 32),
      "Mutation rate (probability of any probe swapping places with another.)")(
      "setup,s", po::value<std::string>()->default_value("sites.csv"),
      "Probe setup file, in CSV format.")(
      "inventory,i", po::value<std::string>()->default_value("inventory.csv"),
      "The inventory file, in CSV format.")(
      "population,p", po::value<size_t>()->default_value(200),
      "Maximum population size.")(
      "age,a", po::value<int>()->default_value(50),
      "Maximum age for a stuck solution; after this many generations, replace "
      "this solution by a new, random one. Set to zero to disable discarding "
      "stuck solutions.")("threads,t", po::value<size_t>()->default_value(8),
                          "Number of concurrent solutions to evaluate.");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  optimizer.setStorageWeight(vm["storageweight"].as<float>());
  optimizer.setRevenueWeight(vm["revenueweight"].as<float>());
  optimizer.setProductionWeight(vm["revenueweight"].as<float>());
  optimizer.setMaxIterations(vm["iterations"].as<size_t>());
  optimizer.setNumOffsprings(vm["offsprings"].as<size_t>());
  optimizer.setMutationRate(vm["mutation"].as<float>());
  optimizer.setMaxPopSize(vm["population"].as<size_t>());
  optimizer.setMaxAge(vm["age"].as<int>());
  optimizer.setMaxThreads(vm["threads"].as<size_t>());

  if (vm.count("help")) {
    cerr << desc << endl;
  }

  return vm;
}

#ifdef __MINGW32__
void doWin32Init() {
  // make sure the RNG is properly seeded
  {
    HCRYPTPROV provider = 0;

    if (!::CryptAcquireContextW(&provider, 0, 0, PROV_RSA_FULL,
                                CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
      throw std::runtime_error{"could not acquire crypto context"};

    const DWORD length = 256;
    BYTE buffer[length];

    if (!::CryptGenRandom(provider, length, buffer)) {
      ::CryptReleaseContext(provider, 0);
      throw std::runtime_error{"could not generate random numbers!"};
    }

    std::seed_seq seq(buffer, buffer + length);
    ProbeArrangement::seedMT(seq);

    if (!::CryptReleaseContext(provider, 0))
      throw std::runtime_error{"could not release crypto context"};
  }

  {
    // set the current working directory
    namespace fs = boost::filesystem;

    std::vector<char> buffer(1024);
    int length = ::GetModuleFileNameA(nullptr, &buffer[0], buffer.size());
    if (::GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
      std::string exeFilename(buffer.begin(), buffer.begin() + length);
      fs::path exePath = fs::path(exeFilename).parent_path();
      spdlog::info("Setting working directory to {}", exePath.string());
      fs::current_path(exePath);
    }
  }
}
#endif

int main(int argc, const char **argv) {
  // Remove log metadata from console output.
  spdlog::set_pattern("%^%v%$");

#ifdef __MINGW32__
  doWin32Init();
#endif

  ProbeOptimizer optimizer;

  auto vm = parseOptions(argc, argv, optimizer);

  if (vm.count("help")) {
    return 0;
  }

  optimizer.loadSites(vm["setup"].as<std::string>());

  if (vm.count("print")) {
    optimizer.loadSetup(vm["setup"].as<std::string>());
    optimizer.printSetup();
    optimizer.printTotals();
    return 0;
  }

  optimizer.loadInventory(vm["inventory"].as<std::string>());

  std::signal(SIGINT, ProbeOptimizer::handleSIGINT);

  optimizer.doHillClimbing();
}
