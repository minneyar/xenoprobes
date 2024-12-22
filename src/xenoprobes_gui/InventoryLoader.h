/**
 * @file InventoryLoader.h
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#ifndef INVENTORYLOADER_H
#define INVENTORYLOADER_H

#include <QJsonValue>
#include <probeoptimizer/probe_optimizer.h>

/**
 * Read probe inventory.
 *
 * This uses the same format as the command-line xenoprobes.
 */
class InventoryLoader {
public:
  [[nodiscard]] static ProbeOptimizer::ProbeInventory
  readInventoryFromJson(const QJsonValue &json);
  static void
  writeInventoryToFile(const ProbeOptimizer::ProbeInventory &probeInventory,
                       const QString &path);
  [[nodiscard]] static QJsonValue
  writeInventoryToJson(const ProbeOptimizer::ProbeInventory &probeInventory);
};

#endif // INVENTORYLOADER_H
