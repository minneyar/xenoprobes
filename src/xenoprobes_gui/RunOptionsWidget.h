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
#include <QSpinBox>
#include <QWidget>

class RunOptionsWidget : public QWidget {
  Q_OBJECT
public:
  explicit RunOptionsWidget(QWidget *parent = nullptr);
  unsigned int storageWeight() { return storageWeight_->value(); }
  void setStorageWeight(int storageWeight) {
    storageWeight_->setValue(storageWeight);
  }
  unsigned int revenueWeight() { return revenueWeight_->value(); }
  void setRevenueWeight(int revenueWeight) {
    revenueWeight_->setValue(revenueWeight);
  }
  unsigned int productionWeight() { return productionWeight_->value(); }
  void setProductionWeight(int productionWeight) {
    productionWeight_->setValue(productionWeight);
  }
  unsigned int iterations() { return iterations_->value(); }
  void setIterations(int iterations) { iterations_->setValue(iterations); }
  unsigned int population() { return population_->value(); }
  void setPopulation(int population) { population_->setValue(population); }
  unsigned int offsprings() { return offsprings_->value(); }
  void setOffsprings(int offsprings) { offsprings_->setValue(offsprings); }
  unsigned int mutation() { return mutation_->value(); }
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
