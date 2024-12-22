/**
 * @file InventoryModel.h
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#ifndef INVENTORYMODEL_H
#define INVENTORYMODEL_H

#include "DataProbe.h"
#include <QAbstractTableModel>
#include <probeoptimizer/probe_optimizer.h>

class InventoryModel : public QAbstractTableModel {
  Q_OBJECT
public:
  enum class Column {
    Name,
    Quantity,
  };
  static constexpr auto kColumnCount = static_cast<int>(Column::Quantity) + 1;

  explicit InventoryModel(ProbeOptimizer* probeOptimizer, QObject *parent = nullptr);
  void setProbeOptimizer(ProbeOptimizer *probeOptimizer);
  [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
  [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation,
                                    int role) const override;
  [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
  [[nodiscard]] QVariant data(const QModelIndex &index,
                              int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
  [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
  static const std::vector<Probe::Id> kProbeIdOrder;
  ProbeOptimizer* probeOptimizer_;
};

#endif // INVENTORYMODEL_H
