/**
 * @file InventoryLoader.cpp
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#include "InventoryLoader.h"

#include <QFile>
#include <QTextStream>

DataProbe::ProbeInventory InventoryLoader::readInventory(const QString &path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::runtime_error("Failed to open file.");
  }
  QTextStream in(&file);
  QHash<QString, unsigned int> inventory;

  // Read data as ProbeId,quantity
  QString line;
  bool ok;
  while (in.readLineInto(&line)) {
    line = line.trimmed();
    if (line.isEmpty() || line.startsWith('#')) {
      // Comment or blank line.
      continue;
    }
    const auto probeId = line.first(line.indexOf(','));
    if (!DataProbe::kAllProbes.contains(probeId)) {
      throw std::runtime_error("Probe not found.");
    }
    const auto quantityStr = line.last(line.size() - line.indexOf(',') - 1);
    const auto quantity = quantityStr.toUInt(&ok, 10);
    if (!ok) {
      throw std::runtime_error("Failed to parse quantity.");
    }
    inventory[probeId] += quantity;
  }

  // Create an inventory that contains all probe types.
  DataProbe::ProbeInventory probeInventory;
  probeInventory.reserve(DataProbe::kAllProbes.size());
  for (const auto &[probeId, probe] : DataProbe::kAllProbes.asKeyValueRange()) {
    if (probe.category == DataProbe::Category::Basic) {
      continue;
    }
    probeInventory.emplace_back(probeId, inventory.value(probeId, 0));
  }
  std::ranges::sort(probeInventory,
                    [](const decltype(probeInventory)::value_type &lhs,
                       const decltype(probeInventory)::value_type &rhs) {
                      return DataProbe::kAllProbes.value(lhs.first) <
                             DataProbe::kAllProbes.value(rhs.first);
                    });
  return probeInventory;
}

void InventoryLoader::writeInventory(
    const DataProbe::ProbeInventory &probeInventory, const QString &path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate |
                 QIODevice::Text)) {
    throw std::runtime_error("Failed to open file.");
  }

  // Write header.
  QTextStream out(&file);
  out << "# This is how the inventory file looks like:\n"
         "# Each line contains:\n"
         "#\n"
         "#      TYPE,QUANTITY\n"
         "#\n"
         "# where the type works like this:\n"
         "#\n"
         "#   M1 = Mining Probe G1\n"
         "#   M2 = Mining Probe G2\n"
         "#   ...\n"
         "#   R1 = Research Probe G1\n"
         "#   R2 = Research Probe G2\n"
         "#   ...\n"
         "#   B1 = Booster Probe G1\n"
         "#   S = Storage Probe\n"
         "#   D = Duplicator Probe\n"
         "#   B = Basic Probe\n"
         "#\n"
         "# Note: you don't need to specify Basic Probes, the program assumes "
         "you have an\n"
         "# infinite amount of them.\n";

  // Write all probes, commenting out those with quantity 0.
  DataProbe::Category lastCategory = DataProbe::Category::Basic;
  for (const auto &[probeId, quantity] : probeInventory) {
    const auto &probe = DataProbe::kAllProbes.value(probeId);
    if (probe.category != lastCategory) {
      out << '\n';
    }
    if (quantity == 0) {
      out << '#';
    }
    out << probeId << ',' << quantity << '\n';
    lastCategory = probe.category;
  }
  out.flush();
}
