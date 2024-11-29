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

/**
 * Read probe inventory.
 *
 * This uses the same format as the command-line xenoprobes.
 */
class InventoryLoader {
public:
  static DataProbe::ProbeInventory readInventory(const QString &path);
  static void writeInventory(const DataProbe::ProbeInventory &probeInventory,
                             const QString &path);
};

#endif // INVENTORYLOADER_H
