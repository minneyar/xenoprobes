/**
 * @file MainWindow.h
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "FnSite.h"
#include "InventoryModel.h"
#include "MiraMap.h"
#include "RunOptionsWidget.h"

#include <QMainWindow>
#include <QTableView>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

private:
  struct Actions {
    QAction *fileOpen = nullptr;
    QAction *fileSave = nullptr;
    QAction *fileSaveAs = nullptr;
    QAction *fileImportSites = nullptr;
    QAction *fileExportSites = nullptr;
    QAction *fileImportInventory = nullptr;
    QAction *fileExportInventory = nullptr;
    QAction *fileExit = nullptr;
  };

  Actions actions;

  struct Widgets {
    MiraMap *miraMap = nullptr;
    QTabBar *tabBar = nullptr;
    QTableView *inventoryTable = nullptr;
    RunOptionsWidget *runOptions = nullptr;
  };

  Widgets widgets_;
  InventoryModel *inventoryModel_;

  void initUi();
  void initActions();
  void saveToPath(const QString &path);

private Q_SLOTS:
  void fileOpen();
  void fileSave();
  void fileSaveAs();
  void fileImportSites();
  void fileExportSites();
  void fileImportInventory();
  void fileExportInventory();
  void tabChanged(int index);
};

#endif // MAINWINDOW_H
