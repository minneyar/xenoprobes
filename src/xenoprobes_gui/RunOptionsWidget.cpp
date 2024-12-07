/**
 * @file RunOptionsWidget.cpp
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#include "RunOptionsWidget.h"

#include <QJsonObject>
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
  connect(storageWeight_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Revenue Weight:"), this));
  layout->addWidget(revenueWeight_);
  revenueWeight_->setMinimum(0);
  revenueWeight_->setMaximum(1000);
  connect(revenueWeight_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Production Weight:"), this));
  layout->addWidget(productionWeight_);
  productionWeight_->setMinimum(0);
  productionWeight_->setMaximum(1000);
  connect(productionWeight_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Iterations:"), this));
  layout->addWidget(iterations_);
  iterations_->setMinimum(1);
  iterations_->setMaximum(5000);
  connect(iterations_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Population:"), this));
  layout->addWidget(population_);
  population_->setMinimum(100);
  population_->setMaximum(500);
  connect(population_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Offspring:"), this));
  layout->addWidget(offsprings_);
  offsprings_->setMinimum(10);
  offsprings_->setMaximum(200);
  connect(offsprings_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Mutation:"), this));
  layout->addWidget(mutation_);
  mutation_->setMinimum(1);
  mutation_->setMaximum(100);
  mutation_->spinBox()->setSuffix(tr("%"));
  connect(mutation_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
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

QJsonValue RunOptionsWidget::optionsToJson(const RunOptions &options) {
  QJsonObject json;
  json["storageWeight"] = options.storageWeight;
  json["revenueWeight"] = options.revenueWeight;
  json["productionWeight"] = options.productionWeight;
  json["iterations"] = options.iterations;
  json["population"] = options.population;
  json["offsprings"] = options.offsprings;
  json["mutation"] = options.mutation;

  return json;
}

RunOptions RunOptionsWidget::optionsFromJson(const QJsonValue &json) {
  if (!json.isObject()) {
    throw std::runtime_error("Bad options format.");
  }
  const auto &jsonObj = json.toObject();
  RunOptions options;

  if (!jsonObj.contains("storageWeight") ||
      !jsonObj["storageWeight"].isDouble()) {
    throw std::runtime_error("Bad storageWeight");
  }
  options.storageWeight = json["storageWeight"].toInt();

  if (!jsonObj.contains("revenueWeight") ||
      !jsonObj["revenueWeight"].isDouble()) {
    throw std::runtime_error("Bad revenueWeight");
  }
  options.revenueWeight = json["revenueWeight"].toInt();

  if (!jsonObj.contains("productionWeight") ||
      !jsonObj["productionWeight"].isDouble()) {
    throw std::runtime_error("Bad productionWeight");
  }
  options.productionWeight = json["productionWeight"].toInt();

  if (!jsonObj.contains("iterations") || !jsonObj["iterations"].isDouble()) {
    throw std::runtime_error("Bad iterations");
  }
  options.iterations = json["iterations"].toInt();

  if (!jsonObj.contains("population") || !jsonObj["population"].isDouble()) {
    throw std::runtime_error("Bad population");
  }
  options.population = json["population"].toInt();

  if (!jsonObj.contains("offsprings") || !jsonObj["offsprings"].isDouble()) {
    throw std::runtime_error("Bad offsprings");
  }
  options.offsprings = json["offsprings"].toInt();

  if (!jsonObj.contains("mutation") || !jsonObj["mutation"].isDouble()) {
    throw std::runtime_error("Bad mutation");
  }
  options.mutation = json["mutation"].toInt();

  return options;
}

void RunOptionsWidget::applyDefaultValues() {
  const RunOptions defaults;
  storageWeight_->setValue(defaults.storageWeight);
  revenueWeight_->setValue(defaults.revenueWeight);
  productionWeight_->setValue(defaults.productionWeight);
  iterations_->setValue(defaults.iterations);
  population_->setValue(defaults.population);
  offsprings_->setValue(defaults.offsprings);
  mutation_->setValue(defaults.mutation);
}
