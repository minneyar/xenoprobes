/**
 * @file RunOptionsWidget.h
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#ifndef RUNOPTIONSWIDGET_H
#define RUNOPTIONSWIDGET_H

#include "SliderWithValWidget.h"
#include <QJsonValue>
#include <QWidget>

class RunOptionsWidget : public QWidget {
  Q_OBJECT
public:
  struct Options {
    int storageWeight = 1000;
    int revenueWeight = 10;
    int productionWeight = 1;
    int iterations = 2000;
    int population = 200;
    int offsprings = 100;
    int mutation = std::round(1.0 / 32 * 100);
  };

  [[nodiscard]] static QJsonValue optionsToJson(const Options &options);
  static Options optionsFromJson(const QJsonValue &json);

  explicit RunOptionsWidget(QWidget *parent = nullptr);
  [[nodiscard]] Options options() const {
    return {
        .storageWeight = storageWeight_->value(),
        .revenueWeight = revenueWeight_->value(),
        .productionWeight = productionWeight_->value(),
        .iterations = iterations_->value(),
        .population = population_->value(),
        .offsprings = offsprings_->value(),
        .mutation = mutation_->value(),
    };
  }
  void setOptions(const Options &options) {
    storageWeight_->setValue(options.storageWeight);
    revenueWeight_->setValue(options.revenueWeight);
    productionWeight_->setValue(options.productionWeight);
    iterations_->setValue(options.iterations);
    population_->setValue(options.population);
    offsprings_->setValue(options.offsprings);
    mutation_->setValue(options.mutation);
  }
  [[nodiscard]] unsigned int storageWeight() { return storageWeight_->value(); }
  void setStorageWeight(int storageWeight) {
    storageWeight_->setValue(storageWeight);
  }
  [[nodiscard]] unsigned int revenueWeight() { return revenueWeight_->value(); }
  void setRevenueWeight(int revenueWeight) {
    revenueWeight_->setValue(revenueWeight);
  }
  [[nodiscard]] unsigned int productionWeight() {
    return productionWeight_->value();
  }
  void setProductionWeight(int productionWeight) {
    productionWeight_->setValue(productionWeight);
  }
  [[nodiscard]] unsigned int iterations() { return iterations_->value(); }
  void setIterations(int iterations) { iterations_->setValue(iterations); }
  [[nodiscard]] unsigned int population() { return population_->value(); }
  void setPopulation(int population) { population_->setValue(population); }
  [[nodiscard]] unsigned int offsprings() { return offsprings_->value(); }
  void setOffsprings(int offsprings) { offsprings_->setValue(offsprings); }
  [[nodiscard]] unsigned int mutation() { return mutation_->value(); }
  void setMutation(int mutation) { mutation_->setValue(mutation); }

public Q_SLOTS:
  void applyDefaultValues();

private:
  SliderWithValWidget *storageWeight_;
  SliderWithValWidget *revenueWeight_;
  SliderWithValWidget *productionWeight_;
  SliderWithValWidget *iterations_;
  SliderWithValWidget *population_;
  SliderWithValWidget *offsprings_;
  SliderWithValWidget *mutation_;
};

#endif // RUNOPTIONSWIDGET_H
