/**
 * @file FnSiteWidget.h
 *
 * @author Dan Keenan
 * @date 11/24/24
 * @copyright GNU GPLv3
 */

#ifndef FNSITEWIDGET_H
#define FNSITEWIDGET_H

#include <QCheckBox>
#include <QLabel>
#include <QStackedWidget>
#include <QWidget>
#include <probeoptimizer/probe.h>
#include <probeoptimizer/site.h>

class FnSiteWidget;

namespace detail {
/**
 * Allow toggling if a site has been visited.
 */
class VisitedWidget : public QWidget {
  Q_OBJECT
public:
  explicit VisitedWidget(const Site::Ptr site, FnSiteWidget *parent = nullptr);
  [[nodiscard]] Site::Ptr site() const { return site_; }
  [[nodiscard]] bool visited() const { return visited_; }
  void setVisited(const bool visited);

Q_SIGNALS:
  void visitedChanged(bool visited);

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  struct Widgets {
    QCheckBox *checkBox = nullptr;
    QLabel *label = nullptr;
  };
  Widgets widgets_;
  const Site::Ptr site_;
  bool visited_ = false;

  [[nodiscard]] FnSiteWidget *parentFnSiteWidget() const;
  void updateCheckbox();
};

/**
 * Show the data probe assigned to this site.
 */
class DataProbeWidget : public QWidget {
  Q_OBJECT
public:
  explicit DataProbeWidget(const Site::Ptr site,
                           FnSiteWidget *parent = nullptr);

  [[nodiscard]] Site::Ptr site() const { return site_; }
  [[nodiscard]] bool visited() const { return visited_; }
  void setVisited(const bool visited);
  [[nodiscard]] const Probe::Ptr dataProbe() const { return dataProbe_; }
  void setDataProbe(Probe::Ptr dataProbe);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  const Site::Ptr site_;
  bool visited_ = false;
  Probe::Ptr dataProbe_ = nullptr;

  [[nodiscard]] FnSiteWidget *parentFnSiteWidget() const;
  [[nodiscard]] QPixmap probeImage() const;
};
} // namespace detail

/**
 * Display info about a FrontierNav Site.
 */
class FnSiteWidget : public QStackedWidget {
  Q_OBJECT

public:
  enum class ViewMode {
    Visited,
    DataProbe,
  };
  Q_ENUM(ViewMode)

  static constexpr auto kSize = 64;

  explicit FnSiteWidget(Site::Ptr site, QWidget *parent = nullptr);

  [[nodiscard]] ViewMode viewMode() const { return viewMode_; }
  void setViewMode(const ViewMode viewMode);
  [[nodiscard]] Site::Ptr site() const { return site_; }
  [[nodiscard]] bool visited() const { return visited_; }
  void setVisited(const bool visited);
  [[nodiscard]] const Probe::Ptr dataProbe() const {
    return dataProbeWidget_->dataProbe();
  }
  void setDataProbe(const Probe::Id &dataProbeId);
  void setDataProbe(const Probe *dataProbe) {
    dataProbeWidget_->setDataProbe(dataProbe);
    updateTooltipText();
  }

Q_SIGNALS:
  void visitedChanged(bool visited);
  void dataProbeChanged(Probe::Ptr dataProbe);

private:
  ViewMode viewMode_ = ViewMode::DataProbe;
  const Site::Ptr site_;
  bool visited_ = false;
  detail::VisitedWidget *visitedWidget_;
  detail::DataProbeWidget *dataProbeWidget_;
  bool tooltipShown_ = false;
  QAction *visitedAction_;
  std::vector<QAction *> setProbeActions_;

  void updateTooltipText();
  void updateSetProbeActions();
};

#endif // FNSITEWIDGET_H
