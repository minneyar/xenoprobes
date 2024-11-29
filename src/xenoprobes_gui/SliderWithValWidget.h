/**
 * @file SliderWithValWidget.h
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#ifndef SLIDERWITHVALWIDGET_H
#define SLIDERWITHVALWIDGET_H

#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

/**
 * A QSlider with a matching QSpinBox.
 */
class SliderWithValWidget : public QWidget {
  Q_OBJECT
public:
  explicit SliderWithValWidget(QWidget *parent = nullptr);

  [[nodiscard]] QSlider *slider() { return slider_; }
  [[nodiscard]] QSpinBox *spinBox() { return spinBox_; }
  void setMinimum(int minimum);
  void setMaximum(int maximum);
  void setValue(int value);
  [[nodiscard]] int value() const { return spinBox_->value(); }

Q_SIGNALS:
  void valueChanged(int value);

private:
  QSlider *slider_;
  QSpinBox *spinBox_;
};

#endif // SLIDERWITHVALWIDGET_H
