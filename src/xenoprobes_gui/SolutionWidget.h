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

class SolutionWidget : public QWidget {
  Q_OBJECT
public:
  explicit SolutionWidget(QWidget *parent = nullptr);
  void setProduction(unsigned int production);
  void setRevenue(unsigned int revenue);
  void setStorage(unsigned int storage);
  void setOres(const QStringList &ores);

private:
  QLabel *productionLabel_;
  QLabel *revenueLabel_;
  QLabel *storageLabel_;
  QLabel *oresLabel_;
};

#endif // SOLUTIONWIDGET_H
