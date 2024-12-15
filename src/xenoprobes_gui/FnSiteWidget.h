/**
 * @file FnSiteWidget.h
 *
 * @author Dan Keenan
 * @date 11/24/24
 * @copyright GNU GPLv3
 */

#ifndef FNSITEWIDGET_H
#define FNSITEWIDGET_H

#include "FnSite.h"
#include <probeoptimizer/probe.h>
#include <QCheckBox>
#include <QLabel>
#include <QStackedWidget>
#include <QWidget>

class FnSiteWidget;

namespace detail {
/**
 * Allow toggling if a site has been visited.
 */
class VisitedWidget : public QWidget {
  Q_OBJECT
public:
  explicit VisitedWidget(const FnSite &site, FnSiteWidget *parent = nullptr);
  [[nodiscard]] FnSite site() const { return site_; }
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
  const FnSite &site_;
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
  explicit DataProbeWidget(const FnSite &site, FnSiteWidget *parent = nullptr);

  [[nodiscard]] FnSite site() const { return site_; }
  [[nodiscard]] bool visited() const { return visited_; }
  void setVisited(const bool visited);
  [[nodiscard]] const Probe *dataProbe() const { return dataProbe_; }
  void setDataProbe(const Probe *dataProbe);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  const FnSite &site_;
  bool visited_ = false;
  const Probe *dataProbe_ = nullptr;

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

  explicit FnSiteWidget(const FnSite &site, QWidget *parent = nullptr);

  [[nodiscard]] ViewMode viewMode() const { return viewMode_; }
  void setViewMode(const ViewMode viewMode);
  [[nodiscard]] FnSite site() const { return site_; }
  [[nodiscard]] bool visited() const { return visited_; }
  void setVisited(const bool visited);
  [[nodiscard]] const Probe *dataProbe() const {
    return dataProbeWidget_->dataProbe();
  }
  void setDataProbe(const Probe::Id &dataProbeId);
  void setDataProbe(const Probe *dataProbe) {
    dataProbeWidget_->setDataProbe(dataProbe);
    updateTooltipText();
  }

Q_SIGNALS:
  void visitedChanged(bool visited);
  void dataProbeChanged(const Probe *data_probe);

private:
  ViewMode viewMode_ = ViewMode::DataProbe;
  const FnSite &site_;
  bool visited_ = false;
  detail::VisitedWidget *visitedWidget_;
  detail::DataProbeWidget *dataProbeWidget_;
  bool tooltipShown_ = false;

  void updateTooltipText();
};

#endif // FNSITEWIDGET_H
