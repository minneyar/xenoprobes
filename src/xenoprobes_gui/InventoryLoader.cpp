/**
 * @file InventoryLoader.cpp
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#include "InventoryLoader.h"

#include <QFile>
#include <QJsonObject>
#include <QTextStream>
#include <probeoptimizer/csv.h>

ProbeInventory InventoryLoader::readInventoryFromFile(const QString &path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::runtime_error("Failed to open file.");
  }
  QTextStream in(&file);
  QHash<Probe::Id, unsigned int> inventory;

  // Read data as ProbeId,quantity
  const auto rows = loadCSV(path.toStdString());
  for (const auto &row : rows) {
    const auto probeId = std::get<std::string>(row.at(0));
    Probe::Ptr probe;
    try {
      probe = Probe::fromString(probeId);
    } catch (const std::out_of_range &) {
      throw std::runtime_error("Probe not found.");
    }
    int quantity;
    try {
      quantity = csvRecordValToInt(row.at(1));
    } catch (const std::out_of_range &) {
      throw std::runtime_error("Failed to parse quantity.");
    } catch (const std::invalid_argument &) {
      throw std::runtime_error("Failed to parse quantity.");
    }
    if (quantity < 0) {
      throw std::runtime_error("Failed to parse quantity.");
    }
    inventory[probe->id] += quantity;
  }

  // Create an inventory that contains all probe types.
  ProbeInventory probeInventory;
  probeInventory.reserve(Probe::ALL.size());
  for (const auto &[probeId, probe] : Probe::ALL) {
    if (probe.category == Probe::Category::Basic) {
      continue;
    }
    probeInventory.emplace_back(probeId, inventory.value(probeId, 0));
  }
  sortProbeInventory(probeInventory);
  return probeInventory;
}

ProbeInventory InventoryLoader::readInventoryFromJson(const QJsonValue &json) {
  if (!json.isObject()) {
    throw std::runtime_error("Bad probe inventory format.");
  }
  const auto &jsonMap = json.toObject();
  ProbeInventory probeInventory;
  probeInventory.reserve(Probe::ALL.size());
  for (const auto &[probeId, probe] : Probe::ALL) {
    if (probe.category == Probe::Category::Basic) {
      continue;
    }
    auto quantity = jsonMap.value(QString::fromStdString(probeId));
    if (quantity.isUndefined()) {
      quantity = 0;
    } else if (!quantity.isDouble()) {
      throw std::runtime_error("Failed to parse quantity.");
    }
    probeInventory.emplace_back(probeId, quantity.toInt());
  }
  sortProbeInventory(probeInventory);

  return probeInventory;
}

void InventoryLoader::writeInventoryToFile(const ProbeInventory &probeInventory,
                                           const QString &path) {
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
  Probe::Category lastCategory = Probe::Category::Basic;
  for (const auto &[probeId, quantity] : probeInventory) {
    const auto &probe = Probe::fromString(probeId);
    if (probe->category != lastCategory) {
      out << '\n';
    }
    if (quantity == 0) {
      out << '#';
    }
    out << QString::fromStdString(probeId) << ',' << quantity << '\n';
    lastCategory = probe->category;
  }
  out.flush();
}

QJsonValue
InventoryLoader::writeInventoryToJson(const ProbeInventory &probeInventory) {
  QJsonObject json;
  for (const auto &[probeId, quantity] : probeInventory) {
    if (quantity > 0) {
      json[QString::fromStdString(probeId)] = static_cast<int>(quantity);
    }
  }
  return json;
}
