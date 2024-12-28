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
#include <ranges>

ProbeOptimizer::ProbeInventory
InventoryLoader::readInventoryFromJson(const QJsonValue &json) {
  if (!json.isObject()) {
    throw std::runtime_error("Bad probe inventory format.");
  }
  const auto &jsonMap = json.toObject();
  ProbeOptimizer::ProbeInventory probeInventory;
  for (const auto probe : Probe::ALL | std::views::values) {
    if (probe->category == Probe::Category::Basic) {
      continue;
    }
    auto quantity = jsonMap.value(QString::fromStdString(probe->id));
    if (quantity.isUndefined()) {
      quantity = 0;
    } else if (!quantity.isDouble()) {
      throw std::runtime_error("Failed to parse quantity.");
    }
    probeInventory.emplace(probe, quantity.toInt());
  }

  return probeInventory;
}

void InventoryLoader::writeInventoryToFile(
    const ProbeOptimizer::ProbeInventory &probeInventory, const QString &path) {
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
  for (const auto [probe, quantity] : probeInventory) {
    if (probe->category != lastCategory) {
      out << '\n';
    }
    if (quantity == 0) {
      out << '#';
    }
    out << QString::fromStdString(probe->id) << ',' << quantity << '\n';
    lastCategory = probe->category;
  }
  out.flush();
}

QJsonValue InventoryLoader::writeInventoryToJson(
    const ProbeOptimizer::ProbeInventory &probeInventory) {
  QJsonObject json;
  for (const auto [probe, quantity] : probeInventory) {
    if (quantity > 0) {
      json[QString::fromStdString(probe->id)] = static_cast<int>(quantity);
    }
  }
  return json;
}
