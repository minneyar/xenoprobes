/**
 * @file InventoryModel.cpp
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#include "InventoryModel.h"
#include "InventoryLoader.h"

#include <QBrush>
#include <QColor>
#include <ranges>

const std::vector<Probe::Id> InventoryModel::kProbeIdOrder{
    // Don't show basic probes in this list.
    "M1",  "M2", "M3", "M4", "M5", "M6", "M7", "M8", "M9",
    "M10", "R1", "R2", "R3", "R4", "R5", "R6", "B1", "B2",
    "D",   "S",  "CF", "CM", "CR", "CD", "CA",
};

InventoryModel::InventoryModel(ProbeOptimizer *probeOptimizer, QObject *parent)
    : QAbstractTableModel(parent), probeOptimizer_(probeOptimizer) {}

void InventoryModel::setProbeOptimizer(ProbeOptimizer *probeOptimizer) {
  beginResetModel();
  probeOptimizer_ = probeOptimizer;
  endResetModel();
}

int InventoryModel::columnCount(const QModelIndex &parent) const {
  return kColumnCount;
}

QVariant InventoryModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  const auto col = static_cast<Column>(section);
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      switch (col) {
      case Column::Name:
        return tr("Name");
      case Column::Quantity:
        return tr("#");
      case Column::Used:
        return tr("Used");
      case Column::Remaining:
        return tr("Remaining");
      }
    }
  }
  return {};
}

int InventoryModel::rowCount(const QModelIndex &parent) const {
  return kProbeIdOrder.size();
}

QVariant InventoryModel::data(const QModelIndex &index, int role) const {
  const auto col = static_cast<Column>(index.column());
  const auto probeId = kProbeIdOrder.at(index.row());
  const auto probe = Probe::fromString(probeId);
  const int quantity = probeOptimizer_->getInventory().at(probe);

  if (role == Qt::DisplayRole) {
    if (col == Column::Name) {
      return dataProbeName(probe);
    } else if (col == Column::Quantity) {
      return quantity;
    } else if (col == Column::Used) {
      const auto usedProbes =
          probeOptimizer_->solution().getSetup().getUsedProbes();
      return usedProbes.at(probe);
    } else if (col == Column::Remaining) {
      const auto usedProbes =
          probeOptimizer_->solution().getSetup().getUsedProbes();
      return quantity - usedProbes.at(probe);
    }
  } else if (role == Qt::EditRole) {
    if (col == Column::Quantity) {
      return quantity;
    }
  } else if (role == Qt::BackgroundRole) {
    const auto usedProbes =
        probeOptimizer_->solution().getSetup().getUsedProbes();
    if (usedProbes.at(probe) > quantity) {
      return QBrush(QColorConstants::Red);
    } else {
      return {};
    }
  }

  return {};
}

bool InventoryModel::setData(const QModelIndex &index, const QVariant &value,
                             int role) {
  bool success = false;
  const auto col = static_cast<Column>(index.column());
  const auto probeId = kProbeIdOrder.at(index.row());
  const auto probe = Probe::fromString(probeId);
  if (role == Qt::EditRole) {
    if (col == Column::Quantity) {
      const auto val = value.toUInt(&success);
      if (success) {
        probeOptimizer_->getInventory()[probe] = val;
      }
    }
  }

  if (success) {
    Q_EMIT(dataChanged(index, index));
  }

  return success;
}

Qt::ItemFlags InventoryModel::flags(const QModelIndex &index) const {
  const auto col = static_cast<Column>(index.column());
  const auto defaults = Qt::ItemNeverHasChildren;
  switch (col) {
  case Column::Name:
    return defaults | Qt::ItemIsEnabled;
  case Column::Quantity:
    return defaults | Qt::ItemIsEnabled | Qt::ItemIsEditable;
  }

  return QAbstractTableModel::flags(index);
}
