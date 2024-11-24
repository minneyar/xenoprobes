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
    : QStackedWidget(parent), site_(site),
      dataProbeWidget_(new detail::DataProbeWidget(site, this)) {
  setAttribute(Qt::WA_NoSystemBackground, true);
  setFixedSize(kSize, kSize);
  addWidget(dataProbeWidget_);
  setViewMode(viewMode_);
}

void FnSiteWidget::setViewMode(const ViewMode viewMode) {
  viewMode_ = viewMode;
  switch (viewMode) {
  case ViewMode::Visited:
    break;
  case ViewMode::DataProbe:
    setCurrentWidget(dataProbeWidget_);
    break;
  }
}

namespace detail {
DataProbeWidget::DataProbeWidget(const FnSite &site, FnSiteWidget *parent)
    : QWidget(parent), site_(site) {
  setAttribute(Qt::WA_NoSystemBackground, true);
}

FnSiteWidget *DataProbeWidget::parentFnSiteWidget() const {
  return static_cast<FnSiteWidget *>(parent());
}

void DataProbeWidget::paintEvent(QPaintEvent *event) {
  const auto image = probeImage();
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  painter.drawPixmap(rect(), image);
  event->accept();
}

QPixmap DataProbeWidget::probeImage() const {
  if (!visited_) {
    return QPixmap(":/probe_icons/notvisited.png");
  }
  if (dataProbe_ == nullptr) {
    return QPixmap(":/probe_icons/none.png");
  }
  return QPixmap(dataProbe_->icon());
}
} // namespace detail
