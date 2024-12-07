/**
 * @file FnSiteWidget.cpp
 *
 * @author Dan Keenan
 * @date 11/24/24
 * @copyright GNU GPLv3
 */

#include "FnSiteWidget.h"

#include <QFile>
#include <QPaintEvent>
#include <QPainter>
#include <QVBoxLayout>

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
    : QWidget(parent), site_(site), probeImageWidget_(new QLabel(this)),
      probeLevelWidget_(new QLabel(this)) {
  setAttribute(Qt::WA_NoSystemBackground, true);
  setVisited(visited_);
  setDataProbe(dataProbe_);
}

FnSiteWidget *DataProbeWidget::parentFnSiteWidget() const {
  return static_cast<FnSiteWidget *>(parent());
}

void DataProbeWidget::setVisited(const bool visited) { visited_ = visited; }
void DataProbeWidget::setDataProbe(const DataProbe *dataProbe) {
  dataProbe_ = dataProbe;
  probeImageWidget_->setPixmap(probeImage());

  // Recalculate size and positioning.
  recalcDimensions();
}

void DataProbeWidget::resizeEvent(QResizeEvent *event) {
  recalcDimensions();
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

void DataProbeWidget::recalcDimensions() {
  // Fill entire area with probe icon.
  probeImageWidget_->setFixedSize(size());

  // Put the level in the top-right corner.
  bool levelIconSet = false;
  if (dataProbe_ != nullptr) {
    const auto probeLevelIconPath =
        QString(":/probe_levels/%1.svg").arg(dataProbe_->level);
    if (QFile::exists(probeLevelIconPath)) {
      QIcon probeLevelIcon(probeLevelIconPath);
      const auto probeLevelSize = size() * 0.3;
      probeLevelWidget_->setFixedSize(probeLevelSize);
      probeLevelWidget_->move(width() - probeLevelWidget_->width(), 0);
      probeLevelWidget_->setPixmap(probeLevelIcon.pixmap(probeLevelSize));
      levelIconSet = true;
    }
  }
  if (!levelIconSet) {
    probeLevelWidget_->clear();
  }
}

} // namespace detail
