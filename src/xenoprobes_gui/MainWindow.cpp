/**
 * @file MainWindow.cpp
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#include "MainWindow.h"
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "FnSite.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    initUi();
}

void MainWindow::initUi()
{
    resize(1024, 768);
    auto central = new QWidget(this);
    setCentralWidget(central);
    auto layout = new QHBoxLayout(central);

    sitesVisited_.clear();
    sitesVisited_.insert(FnSite::kAllSites.keyBegin(), FnSite::kAllSites.keyEnd());

    widgets_.miraMap = new MiraMap(&sitesVisited_, central);
    layout->addWidget(widgets_.miraMap);
    widgets_.miraMap->show();
}
