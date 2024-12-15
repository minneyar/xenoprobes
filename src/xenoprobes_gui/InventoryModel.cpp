/**
 * @file InventoryModel.cpp
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#include "InventoryModel.h"
#include "InventoryLoader.h"
#include <ranges>

InventoryModel::InventoryModel(QObject *parent) : QAbstractTableModel(parent) {
  probeInventory_.reserve(Probe::ALL.size());
  for (const auto &probe : Probe::ALL | std::views::values) {
    if (probe.category == Probe::Category::Basic) {
      continue;
    }
    probeInventory_.emplace_back(probe.id, 0);
  }
  sortProbeInventory(probeInventory_);
}

void InventoryModel::setProbeInventory(const ProbeInventory &probeInventory) {
  beginResetModel();
  probeInventory_ = probeInventory;
  sortProbeInventory(probeInventory_);
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
      }
    }
  }
  return {};
}

int InventoryModel::rowCount(const QModelIndex &parent) const {
  return probeInventory_.size();
}

QVariant InventoryModel::data(const QModelIndex &index, int role) const {
  const auto col = static_cast<Column>(index.column());
  const auto [probeId, quantity] = probeInventory().at(index.row());
  const auto probe = Probe::fromString(probeId);

  if (role == Qt::DisplayRole) {
    if (col == Column::Name) {
      return dataProbeName(probe);
    } else if (col == Column::Quantity) {
      return quantity;
    }
  } else if (role == Qt::EditRole) {
    if (col == Column::Quantity) {
      return quantity;
    }
  }

  return {};
}

bool InventoryModel::setData(const QModelIndex &index, const QVariant &value,
                             int role) {
  bool success = false;
  const auto col = static_cast<Column>(index.column());
  if (role == Qt::EditRole) {
    if (col == Column::Quantity) {
      const auto val = value.toUInt(&success);
      if (success) {
        probeInventory_[index.row()].second = val;
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
  const auto defaults = Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
  switch (col) {
  case Column::Name:
    return defaults;
  case Column::Quantity:
    return defaults | Qt::ItemIsEditable;
  }

  return QAbstractTableModel::flags(index);
}
