/**
 * @file MainWindow.h
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "InventoryModel.h"
#include "MiraMap.h"
#include "RunOptionsWidget.h"
#include "SolutionWidget.h"
#include "SolverRunner.h"

#include <QMainWindow>
#include <QProgressDialog>
#include <QTableView>
#include <probeoptimizer/probe_optimizer.h>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  struct Actions {
    QAction *fileOpen = nullptr;
    QMenu *fileRecent = nullptr;
    QAction *fileSave = nullptr;
    QAction *fileSaveAs = nullptr;
    QAction *fileImportSites = nullptr;
    QAction *fileExportSites = nullptr;
    QAction *fileImportInventory = nullptr;
    QAction *fileExportInventory = nullptr;
    QAction *fileExit = nullptr;
    QAction *viewZoomIn = nullptr;
    QAction *viewZoomOut = nullptr;
    QAction *viewZoomAll = nullptr;
  };

  Actions actions;

  struct Widgets {
    MiraMap *miraMap = nullptr;
    QTabBar *tabBar = nullptr;
    QTableView *inventoryTable = nullptr;
    RunOptionsWidget *runOptions = nullptr;
    SolutionWidget *solutionWidget = nullptr;
  };

  Widgets widgets_;
  InventoryModel *inventoryModel_;
  QProgressDialog *progressDialog_ = nullptr;
  QElapsedTimer solverStopwatch_;
  SolverRunner *solverRunner_ = nullptr;
  ProbeOptimizer probeOptimizer_;
  bool shownForFirstTime = false;

  void initUi();
  void initActions();
  void updateWindowTitle();
  void addRecentDocument(const QString &path);
  void updateRecentDocuments();
  void openFromPath(const QString &path);
  void saveToPath(const QString &path);
  bool safeToCloseFile();

private Q_SLOTS:
  void fileOpen();
  void fileSave();
  void fileSaveAs();
  void fileImportSites();
  void fileExportSites();
  void fileImportInventory();
  void fileExportInventory();
  void tabChanged(int index);
  void dataChanged();
  void probeMapChanged();
  void solve();
  void progress(unsigned long iter, double bestScore, double worstScore,
                unsigned long killed);
  void solved(ProbeArrangement probeArrangement);
};

#endif // MAINWINDOW_H
