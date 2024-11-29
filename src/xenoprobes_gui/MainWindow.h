/**
 * @file MainWindow.h
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "FnSite.h"
#include "MiraMap.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    struct Actions
    {
        QAction* fileImportSites = nullptr;
        QAction* fileExportSites = nullptr;
        QAction* fileImportInventory = nullptr;
        QAction* fileExportInventory = nullptr;
        QAction* fileExit = nullptr;
    };

    Actions actions;

    struct Widgets
    {
        MiraMap* miraMap = nullptr;
        QTabBar* tabBar = nullptr;
    };

    Widgets widgets_;
    FnSite::IdList sitesVisited_;
    DataProbe::ProbeInventory probeInventory_;

    void initUi();
    void initActions();

private Q_SLOTS:
    void fileImportSites();
    void fileExportSites();
    void fileImportInventory();
    void fileExportInventory();
    void tabChanged(int index);
};


#endif //MAINWINDOW_H
