/**
 * @file InventoryModel.cpp
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#include "InventoryModel.h"

#include "InventoryLoader.h"

InventoryModel::InventoryModel(QObject *parent) {
  probeInventory_.reserve(DataProbe::kAllProbes.size());
  for (const auto &probe : DataProbe::kAllProbes) {
    if (probe.category == DataProbe::Category::Basic) {
      continue;
    }
    probeInventory_.emplace_back(probe.id, 0);
  }
  sortProbeInventory();
}

void InventoryModel::setProbeInventory(
    const DataProbe::ProbeInventory &probeInventory) {
  beginResetModel();
  probeInventory_ = probeInventory;
  sortProbeInventory();
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
  const auto &probe = DataProbe::kAllProbes.value(probeId);

  if (role == Qt::DisplayRole) {
    if (col == Column::Name) {
      return probe.name;
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

void InventoryModel::sortProbeInventory() {
  std::ranges::sort(probeInventory_,
                    [](const decltype(probeInventory_)::value_type &lhs,
                       const decltype(probeInventory_)::value_type &rhs) {
                      return DataProbe::kAllProbes.value(lhs.first) <
                             DataProbe::kAllProbes.value(rhs.first);
                    });
}
