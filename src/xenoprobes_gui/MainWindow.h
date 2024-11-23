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
    struct Widgets
    {
        MiraMap* miraMap = nullptr;
    };
    Widgets widgets_;
    FnSite::IdList sitesVisited_;

    void initUi();
};


#endif //MAINWINDOW_H
