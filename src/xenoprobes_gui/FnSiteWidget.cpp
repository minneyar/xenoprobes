/**
 * @file FnSiteWidget.cpp
 *
 * @author Dan Keenan
 * @date 11/24/24
 * @copyright GNU GPLv3
 */

#include "FnSiteWidget.h"

#include "DataProbe.h"
#include <QFile>
#include <QLocale>
#include <QPaintEvent>
#include <QPainter>
#include <QSvgRenderer>
#include <QVBoxLayout>
#include <ranges>

FnSiteWidget::FnSiteWidget(const Site::Ptr site, QWidget *parent)
    : QStackedWidget(parent), site_(site),
      visitedWidget_(new detail::VisitedWidget(site, this)),
      dataProbeWidget_(new detail::DataProbeWidget(site, this)) {
  setFixedSize(kSize, kSize);

  visitedAction_ = new QAction(tr("Visited"), this);
  visitedAction_->setCheckable(true);
  visitedAction_->setChecked(visited_);
  connect(visitedAction_, &QAction::triggered, [this](bool checked) {
    visited_ = checked;
    visitedWidget_->setVisited(checked);
    dataProbeWidget_->setVisited(checked);
    updateTooltipText();
    updateSetProbeActions();
    Q_EMIT(visitedChanged(checked));
  });
  addAction(visitedAction_);
  addWidget(visitedWidget_);
  visitedWidget_->setVisited(visited_);
  connect(visitedWidget_, &detail::VisitedWidget::visitedChanged,
          [this](bool visited) {
            visited_ = visited;
            visitedAction_->setChecked(visited);
            dataProbeWidget_->setVisited(visited);
            updateTooltipText();
            updateSetProbeActions();
            Q_EMIT(visitedChanged(visited));
          });

  addWidget(dataProbeWidget_);
  dataProbeWidget_->setVisited(visited_);

  setViewMode(viewMode_);
  updateTooltipText();

  // Actions to change probe.
  auto sep = new QAction(this);
  sep->setSeparator(true);
  addAction(sep);
  setProbeActions_.reserve(Probe::ALL_SORTED.size());
  for (const auto probe : Probe::ALL_SORTED) {
    auto *action = new QAction(this);
    action->setIcon(QIcon(dataProbeIcon(probe)));
    action->setText(dataProbeName(probe));
    connect(action, &QAction::triggered, [this, probe]() {
      setDataProbe(probe);
      Q_EMIT(dataProbeChanged(probe));
    });
    addAction(action);
    setProbeActions_.push_back(action);
  }
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
  visitedAction_->setChecked(visited);
  visitedWidget_->setVisited(visited);
  dataProbeWidget_->setVisited(visited);
  updateTooltipText();
  updateSetProbeActions();
}

void FnSiteWidget::setDataProbe(const Probe::Id &dataProbeId) {
  const auto dataProbe = Probe::fromString(dataProbeId);
  setDataProbe(dataProbe);
}

void FnSiteWidget::updateTooltipText() {
  const auto ores = site_->getOre();
  QStringList oreStrings;
  std::transform(
      ores.begin(), ores.end(), std::back_inserter(oreStrings),
      [](const Ore::Ptr ore) { return QString::fromStdString(ore->name); });
  setToolTip(
      tr("<strong>FN%1</strong><br>"
         "%2<br>"
         "<table>"
         "<tr><th align=\"right\">Production:</th><td "
         "align=\"left\">%3</td></tr>"
         "<tr><th align=\"right\">Revenue:</th><td align=\"left\">%4</td></tr>"
         "<tr><th align=\"right\">Combat:</th><td align=\"left\">%5</td></tr>"
         "<tr><th align=\"right\">Ore:</th><td align=\"left\">%6</td></tr>"
         "</table>")
          .arg(site_->name)
          .arg(dataProbe() == nullptr ? tr("No probe")
                                      : dataProbeName(dataProbe()))
          .arg(Site::gradeToChar(site_->production))
          .arg(Site::gradeToChar(site_->revenue))
          .arg(Site::gradeToChar(site_->combat))
          .arg(QLocale().createSeparatedList(oreStrings)));
}

void FnSiteWidget::updateSetProbeActions() {
  for (auto action : setProbeActions_) {
    action->setEnabled(visited_);
  }
}

namespace detail {
VisitedWidget::VisitedWidget(const Site::Ptr site, FnSiteWidget *parent)
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
  connect(widgets_.checkBox,
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
          &QCheckBox::checkStateChanged,
          [this](Qt::CheckState checkState)
#else
    &QCheckBox::stateChanged,
          [this](int checkState)
#endif
          {
            visited_ = checkState == Qt::Checked;
            Q_EMIT(visitedChanged(visited_));
          });

  // Label
  widgets_.label = new QLabel(this);
  layout->addWidget(widgets_.label);
  widgets_.label->setText(QString("<b><big>%1</big></b>").arg(site_->name));
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

DataProbeWidget::DataProbeWidget(const Site::Ptr site, FnSiteWidget *parent)
    : QWidget(parent), site_(site) {
  setAttribute(Qt::WA_NoSystemBackground, true);
}

FnSiteWidget *DataProbeWidget::parentFnSiteWidget() const {
  return static_cast<FnSiteWidget *>(parent());
}

void DataProbeWidget::setVisited(const bool visited) {
  visited_ = visited;
  update();
}
void DataProbeWidget::setDataProbe(const Probe *dataProbe) {
  dataProbe_ = dataProbe;
  update();
}

void DataProbeWidget::paintEvent(QPaintEvent *event) {
  const auto image = probeImage();
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);

  // Probe image.
  painter.drawPixmap(rect(), image);

  // Probe level.
  if (dataProbe_ != nullptr && dataProbe_->level > 0) {
    QSvgRenderer svgRenderer(
        QString(":/probe_levels/%1.svg").arg(dataProbe_->level));
    svgRenderer.setAspectRatioMode(Qt::KeepAspectRatio);
    svgRenderer.render(&painter);
  }

  event->accept();
}

QPixmap DataProbeWidget::probeImage() const {
  if (!visited_) {
    return QPixmap(":/probe_icons/notvisited.png");
  }
  if (dataProbe_ == nullptr) {
    return QPixmap(":/probe_icons/none.png");
  }
  return QPixmap(dataProbeIcon(dataProbe_));
}

} // namespace detail
