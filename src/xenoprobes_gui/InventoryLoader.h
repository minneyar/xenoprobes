/**
 * @file InventoryLoader.h
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#ifndef INVENTORYLOADER_H
#define INVENTORYLOADER_H

#include "DataProbe.h"
#include <QJsonValue>

/**
 * Read probe inventory.
 *
 * This uses the same format as the command-line xenoprobes.
 */
class InventoryLoader {
public:
  [[nodiscard]] static ProbeInventory
  readInventoryFromFile(const QString &path);
  [[nodiscard]] static ProbeInventory
  readInventoryFromJson(const QJsonValue &json);
  static void writeInventoryToFile(const ProbeInventory &probeInventory,
                                   const QString &path);
  [[nodiscard]] static QJsonValue
  writeInventoryToJson(const ProbeInventory &probeInventory);
};

#endif // INVENTORYLOADER_H
