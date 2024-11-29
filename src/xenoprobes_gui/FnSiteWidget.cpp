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
#include <qboxlayout.h>

FnSiteWidget::FnSiteWidget(const FnSite &site, QWidget *parent)
    : QStackedWidget(parent), site_(site),
      visitedWidget_(new detail::VisitedWidget(site, this)),
      dataProbeWidget_(new detail::DataProbeWidget(site, this)) {
  setFixedSize(kSize, kSize);

  addWidget(visitedWidget_);
  visitedWidget_->setVisited(visited_);
  connect(visitedWidget_, &detail::VisitedWidget::visitedChanged, this,
          &FnSiteWidget::visitedChanged);
  addWidget(dataProbeWidget_);
  dataProbeWidget_->setVisited(visited_);

  setViewMode(viewMode_);
}

void FnSiteWidget::setViewMode(const ViewMode viewMode) {
  viewMode_ = viewMode;
  switch (viewMode) {
  case ViewMode::Visited:
    setAttribute(Qt::WA_NoSystemBackground, false);
    setCurrentWidget(visitedWidget_);
    break;
  case ViewMode::DataProbe:
    setAttribute(Qt::WA_NoSystemBackground, true);
    setCurrentWidget(dataProbeWidget_);
    break;
  }
}

void FnSiteWidget::setVisited(const bool visited) {
  visited_ = visited;
  visitedWidget_->setVisited(visited);
  dataProbeWidget_->setVisited(visited);
}

void FnSiteWidget::setDataProbe(const DataProbe::Id &dataProbeId) {
  const auto dataProbe = DataProbe::kAllProbes.find(dataProbeId);
  if (dataProbe == DataProbe::kAllProbes.end()) {
    throw std::runtime_error("Data probe not found.");
  }
  setDataProbe(&dataProbe.value());
}

namespace detail {
VisitedWidget::VisitedWidget(const FnSite &site, FnSiteWidget *parent)
    : QWidget(parent), site_(site) {
  auto *layout = new QVBoxLayout(this);

  // Checkbox
  auto checkLayout = new QHBoxLayout();
  layout->addLayout(checkLayout);
  checkLayout->addStretch();
  widgets_.checkBox = new QCheckBox(this);
  checkLayout->addWidget(widgets_.checkBox);
  checkLayout->addStretch();
  updateCheckbox();
  connect(widgets_.checkBox, &QCheckBox::checkStateChanged,
          [this](Qt::CheckState checkState) {
            visited_ = checkState == Qt::Checked;
            Q_EMIT(visitedChanged(visited_));
          });

  // Label
  widgets_.label = new QLabel(this);
  layout->addWidget(widgets_.label);
  widgets_.label->setText(QString("<b><big>%1</big></b>").arg(site_.id));
  widgets_.label->setAlignment(Qt::AlignCenter);
  widgets_.label->setTextFormat(Qt::RichText);
}

void VisitedWidget::setVisited(const bool visited) {
  visited_ = visited;
  updateCheckbox();
}
void VisitedWidget::mousePressEvent(QMouseEvent *event) {
  // Needed in order to receive mouse release events.
  event->accept();
}
void VisitedWidget::mouseReleaseEvent(QMouseEvent *event) {
  setVisited(!visited_);
  event->accept();
}

void VisitedWidget::updateCheckbox() {
  widgets_.checkBox->setCheckState(visited_ ? Qt::Checked : Qt::Unchecked);
}

FnSiteWidget *VisitedWidget::parentFnSiteWidget() const {
  return static_cast<FnSiteWidget *>(parent());
}

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
