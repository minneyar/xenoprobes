/**
 * @file FnSiteWidget.h
 *
 * @author Dan Keenan
 * @date 11/24/24
 * @copyright GNU GPLv3
 */

#ifndef FNSITEWIDGET_H
#define FNSITEWIDGET_H

#include "DataProbe.h"
#include "FnSite.h"
#include <QStackedWidget>
#include <QWidget>

class FnSiteWidget;

namespace detail {

class DataProbeWidget : public QWidget {
  Q_OBJECT
public:
  explicit DataProbeWidget(const FnSite &site, FnSiteWidget *parent = nullptr);

  [[nodiscard]] FnSite site() const { return site_; }
  [[nodiscard]] bool visited() const { return visited_; }
  void setVisited(const bool visited) {
    visited_ = visited;
    update();
  }
  [[nodiscard]] const DataProbe *dataProbe() const { return dataProbe_; }
  void setDataProbe(const DataProbe *data_probe) {
    dataProbe_ = data_probe;
    update();
  }

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  const FnSite &site_;
  bool visited_ = false;
  const DataProbe *dataProbe_ = nullptr;

  [[nodiscard]] FnSiteWidget *parentFnSiteWidget() const;

  [[nodiscard]] QPixmap probeImage() const;
};
} // namespace detail

class FnSiteWidget : public QStackedWidget {
  Q_OBJECT

public:
  enum class ViewMode {
    Visited,
    DataProbe,
  };

  explicit FnSiteWidget(const FnSite &site, QWidget *parent = nullptr);

  [[nodiscard]] ViewMode viewMode() const { return viewMode_; }
  void setViewMode(const ViewMode viewMode);
  [[nodiscard]] FnSite site() const { return site_; }
  [[nodiscard]] bool visited() const { return visited_; }
  void setVisited(const bool visited) {
    visited_ = visited;
    dataProbeWidget_->setVisited(visited);
  }
  [[nodiscard]] const DataProbe *dataProbe() const {
    return dataProbeWidget_->dataProbe();
  }
  void setDataProbe(const DataProbe *data_probe) {
    dataProbeWidget_->setDataProbe(data_probe);
  }

Q_SIGNALS:
  void visitedChanged(bool visited);
  void dataProbeChanged(const DataProbe *data_probe);

private:
  static constexpr auto kSize = 64;
  ViewMode viewMode_ = ViewMode::DataProbe;
  const FnSite &site_;
  bool visited_ = false;

  detail::DataProbeWidget *dataProbeWidget_;
};

#endif // FNSITEWIDGET_H
