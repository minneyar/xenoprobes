/**
 * @file SolutionWidget.cpp
 *
 * @author Dan Keenan
 * @date 12/8/24
 * @copyright GNU GPLv3
 */

#include "SolutionWidget.h"
#include <QFormLayout>
#include <QPushButton>

SolutionWidget::SolutionWidget(QWidget *parent)
    : QWidget(parent), productionLabel_(new QLabel(this)),
      revenueLabel_(new QLabel(this)), storageLabel_(new QLabel(this)),
      oresLabel_(new QLabel(this)) {
  auto *layout = new QFormLayout(this);
  layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
  layout->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
  layout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
  auto *runButton = new QPushButton(tr("Solve"), this);
  connect(runButton, &QPushButton::clicked, this,
          &SolutionWidget::simulateRequested);
  layout->addRow(runButton);
  layout->addRow(tr("Production:"), productionLabel_);
  layout->addRow(tr("Revenue:"), revenueLabel_);
  layout->addRow(tr("Storage:"), storageLabel_);
  layout->addRow(tr("Ores:"), oresLabel_);
  oresLabel_->setTextFormat(Qt::RichText);
}

void SolutionWidget::setSolution(const ProbeArrangement &probeArrangement) {
  productionLabel_->setText(
      QString::number(probeArrangement.getTotalProduction()));
  revenueLabel_->setText(QString::number(probeArrangement.getTotalRevenue()));
  storageLabel_->setText(QString::number(probeArrangement.getTotalStorage()));
  QStringList ores;
  for (const auto &ore : probeArrangement.getOres()) {
    ores.push_back(QString::fromStdString(ore));
  }
  oresLabel_->setText(ores.join("<br>"));
}
