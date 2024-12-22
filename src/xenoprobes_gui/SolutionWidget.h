/**
 * @file SolutionWidget.h
 *
 * @author Dan Keenan
 * @date 12/8/24
 * @copyright GNU GPLv3
 */

#ifndef SOLUTIONWIDGET_H
#define SOLUTIONWIDGET_H

#include <QLabel>
#include <QWidget>
#include <probeoptimizer/probe_arrangement.h>

class SolutionWidget : public QWidget {
  Q_OBJECT
public:
  explicit SolutionWidget(QWidget *parent = nullptr);
  void setSolution(const ProbeArrangement &probeArrangement);

private:
  QLabel *productionLabel_;
  QLabel *revenueLabel_;
  QLabel *storageLabel_;
  QLabel *oresLabel_;
};

#endif // SOLUTIONWIDGET_H
