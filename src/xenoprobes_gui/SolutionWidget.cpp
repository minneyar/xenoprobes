/**
 * @file SolutionWidget.cpp
 *
 * @author Dan Keenan
 * @date 12/8/24
 * @copyright GNU GPLv3
 */

#include "SolutionWidget.h"
#include <QFormLayout>

SolutionWidget::SolutionWidget(QWidget *parent)
    : QWidget(parent), productionLabel_(new QLabel(this)),
      revenueLabel_(new QLabel(this)), storageLabel_(new QLabel(this)),
      oresLabel_(new QLabel(this)) {
  auto *layout = new QFormLayout(this);
  layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
  layout->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
  layout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
  layout->addRow(tr("Production:"), productionLabel_);
  layout->addRow(tr("Revenue:"), revenueLabel_);
  layout->addRow(tr("Storage:"), storageLabel_);
  layout->addRow(tr("Ores:"), oresLabel_);
  oresLabel_->setTextFormat(Qt::RichText);
}

void SolutionWidget::setProduction(unsigned int production) {
  productionLabel_->setText(QString::number(production));
}

void SolutionWidget::setRevenue(unsigned int revenue) {
  revenueLabel_->setText(QString::number(revenue));
}

void SolutionWidget::setStorage(unsigned int storage) {
  storageLabel_->setText(QString::number(storage));
}

void SolutionWidget::setOres(const QStringList &ores) {
  oresLabel_->setText(ores.join("<br>"));
}
