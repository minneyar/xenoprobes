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
#include <QWidget>

class FnSiteWidget : public QWidget {
  Q_OBJECT

public:
  explicit FnSiteWidget(const FnSite &site, QWidget *parent = nullptr);

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

Q_SIGNALS:
  void visitedChanged(bool visited);
  void dataProbeChanged(const DataProbe *data_probe);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  static constexpr auto kSize = 64;
  const FnSite &site_;
  bool visited_ = false;
  const DataProbe *dataProbe_ = nullptr;

  [[nodiscard]] QPixmap probeImage() const;
};

#endif // FNSITEWIDGET_H
