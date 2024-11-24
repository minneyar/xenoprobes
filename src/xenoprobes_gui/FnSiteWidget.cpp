/**
 * @file FnSiteWidget.cpp
 *
 * @author Dan Keenan
 * @date 11/24/24
 * @copyright GNU GPLv3
 */

#include "FnSiteWidget.h"
#include <QPaintEvent>
#include <QPainter>

FnSiteWidget::FnSiteWidget(const FnSite &site, QWidget *parent)
    : QWidget(parent), site_(site) {
  setFixedSize(kSize, kSize);
  setAttribute(Qt::WA_NoSystemBackground);
}
void FnSiteWidget::paintEvent(QPaintEvent *event) {
  const auto image = probeImage();

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  painter.drawPixmap(rect(), image);

  event->accept();
}

QPixmap FnSiteWidget::probeImage() const {
  if (!visited_) {
    return QPixmap(":/probe_icons/notvisited.png");
  }
  if (dataProbe_ == nullptr) {
    return QPixmap(":/probe_icons/none.png");
  }
  return QPixmap(dataProbe_->icon());
}
