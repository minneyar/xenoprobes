/**
 * @file RunOptionsWidget.cpp
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#include "RunOptionsWidget.h"

#include <QPushButton>
#include <QVBoxLayout>

RunOptionsWidget::RunOptionsWidget(QWidget *parent)
    : QWidget(parent), storageWeight_(new SliderWithValWidget(this)),
      revenueWeight_(new SliderWithValWidget(this)),
      productionWeight_(new SliderWithValWidget(this)),
      iterations_(new SliderWithValWidget(this)),
      population_(new SliderWithValWidget(this)),
      offsprings_(new SliderWithValWidget(this)),
      mutation_(new SliderWithValWidget(this)) {
  auto *layout = new QVBoxLayout(this);
  layout->addWidget(new QLabel(tr("Storage Weight:"), this));
  layout->addWidget(storageWeight_);
  storageWeight_->setMinimum(0);
  storageWeight_->setMaximum(1000);
  layout->addWidget(new QLabel(tr("Revenue Weight:"), this));
  layout->addWidget(revenueWeight_);
  revenueWeight_->setMinimum(0);
  revenueWeight_->setMaximum(1000);
  layout->addWidget(new QLabel(tr("Production Weight:"), this));
  layout->addWidget(productionWeight_);
  productionWeight_->setMinimum(0);
  productionWeight_->setMaximum(1000);
  layout->addWidget(new QLabel(tr("Iterations:"), this));
  layout->addWidget(iterations_);
  iterations_->setMinimum(1);
  iterations_->setMaximum(5000);
  layout->addWidget(new QLabel(tr("Population:"), this));
  layout->addWidget(population_);
  population_->setMinimum(100);
  population_->setMaximum(500);
  layout->addWidget(new QLabel(tr("Offspring:"), this));
  layout->addWidget(offsprings_);
  offsprings_->setMinimum(10);
  offsprings_->setMaximum(200);
  layout->addWidget(new QLabel(tr("Mutation:"), this));
  layout->addWidget(mutation_);
  mutation_->setMinimum(1);
  mutation_->setMaximum(100);
  mutation_->spinBox()->setSuffix(tr("%"));
  applyDefaultValues();

  auto applyDefaultsBtn = new QPushButton(tr("Restore Defaults"), this);
  layout->addWidget(applyDefaultsBtn);
  connect(applyDefaultsBtn, &QPushButton::clicked, this,
          &RunOptionsWidget::applyDefaultValues);

  // Match spinbox widths for aesthetics.
  const auto allWidgets = {
      storageWeight_, revenueWeight_, productionWeight_, iterations_,
      population_,    offsprings_,    mutation_,
  };
  const auto maxWidth =
      std::ranges::max(allWidgets,
                       [](SliderWithValWidget *lhs, SliderWithValWidget *rhs) {
                         return lhs->spinBox()->width() <
                                rhs->spinBox()->width();
                       })
          ->spinBox()
          ->width();
  for (auto *widget : allWidgets) {
    widget->spinBox()->setMinimumWidth(maxWidth);
  }
}

void RunOptionsWidget::applyDefaultValues() {
  storageWeight_->setValue(1000);
  revenueWeight_->setValue(10);
  productionWeight_->setValue(1);
  iterations_->setValue(2000);
  population_->setValue(200);
  offsprings_->setValue(100);
  mutation_->setValue(std::round((1.0 / 32) * 100));
}
