/**
 * @file SliderWithValWidget.cpp
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#include "SliderWithValWidget.h"

#include <QHBoxLayout>

SliderWithValWidget::SliderWithValWidget(QWidget *parent)
    : QWidget(parent), slider_(new QSlider(Qt::Horizontal, this)),
      spinBox_(new QSpinBox(this)) {
  auto *layout = new QHBoxLayout(this);
  layout->addWidget(slider_);
  layout->addWidget(spinBox_);
  connect(slider_, &QSlider::valueChanged, spinBox_, &QSpinBox::setValue);
  connect(slider_, &QSlider::valueChanged, this,
          &SliderWithValWidget::valueChanged);
  connect(spinBox_, &QSpinBox::valueChanged, slider_, &QSlider::setValue);
  connect(spinBox_, &QSpinBox::valueChanged, this,
          &SliderWithValWidget::valueChanged);
}

void SliderWithValWidget::setMinimum(int minimum) {
  slider_->setMinimum(minimum);
  spinBox_->setMinimum(minimum);
}

void SliderWithValWidget::setMaximum(int maximum) {
  slider_->setMaximum(maximum);
  spinBox_->setMaximum(maximum);
}

void SliderWithValWidget::setValue(int value) {
  slider_->setValue(value);
  spinBox_->setValue(value);
}
