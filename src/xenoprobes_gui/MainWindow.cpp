/**
 * @file MainWindow.cpp
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#include "MainWindow.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QTabBar>

#include "FnSite.h"
#include "SiteListLoader.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) { initUi(); }

void MainWindow::initUi() {
  resize(1024, 768);
  initActions();

  auto central = new QWidget(this);
  setCentralWidget(central);
  auto layout = new QHBoxLayout(central);

  // Menubar
  auto *menuFile = menuBar()->addMenu(tr("&File"));
  // File menu
  menuFile->addAction(actions.fileImportSites);
  menuFile->addAction(actions.fileExportSites);
  menuFile->addSeparator();
  menuFile->addAction(actions.fileExit);

  // Start with all sites enabled.
  sitesVisited_.clear();
  sitesVisited_.insert(FnSite::kAllSites.keyBegin(),
                       FnSite::kAllSites.keyEnd());

  // Map
  auto mapLayout = new QVBoxLayout();
  layout->addLayout(mapLayout);
  widgets_.miraMap = new MiraMap(&sitesVisited_, central);
  mapLayout->addWidget(widgets_.miraMap);
  widgets_.miraMap->show();

  // Tabs
  widgets_.tabBar = new QTabBar(central);
  mapLayout->addWidget(widgets_.tabBar);
  widgets_.tabBar->setShape(QTabBar::RoundedSouth);
  auto tabSitesVisited = widgets_.tabBar->addTab(tr("Sites Visited"));
  widgets_.tabBar->setTabData(
      tabSitesVisited, QVariant::fromValue(FnSiteWidget::ViewMode::Visited));
  auto tabDataProbes = widgets_.tabBar->addTab(tr("Data Probes"));
  widgets_.tabBar->setTabData(
      tabDataProbes, QVariant::fromValue(FnSiteWidget::ViewMode::DataProbe));
  connect(widgets_.tabBar, &QTabBar::currentChanged, this,
          &MainWindow::tabChanged);
  tabChanged(widgets_.tabBar->currentIndex());
}

void MainWindow::initActions() {
  // File
  // Import Sites
  actions.fileImportSites =
      new QAction(QIcon::fromTheme("document-import"), "Import Sites", this);
  connect(actions.fileImportSites, &QAction::triggered, this,
          &MainWindow::fileImportSites);
  // Export Sites
  actions.fileExportSites =
      new QAction(QIcon::fromTheme("document-export"), "Export Sites", this);
  connect(actions.fileExportSites, &QAction::triggered, this,
          &MainWindow::fileExportSites);
  // Exit
  actions.fileExit =
      new QAction(QIcon::fromTheme("application-exit"), "E&xit", this);
  connect(actions.fileExit, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::fileImportSites() {
  QFileDialog dialog(this, tr("Import Sites..."));
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setNameFilter(tr("Sites (*.csv)"));
  if (dialog.exec() != QDialog::Accepted) {
    return;
  }
  const auto filenames = dialog.selectedFiles();
  try {
    const auto newIds = SiteListLoader::readSiteList(filenames.first());
    sitesVisited_ = newIds;
    widgets_.miraMap->setSitesVisited(&sitesVisited_);
  } catch (const std::exception &) {
    QMessageBox::critical(this, tr("Failed to open file"),
                          tr("Could not open %1").arg(filenames.first()));
  }
}

void MainWindow::fileExportSites() {
  QFileDialog dialog(this, tr("Export Sites..."));
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setNameFilter(tr("Sites (*.csv)"));
  if (dialog.exec() != QDialog::Accepted) {
    return;
  }
  const auto filenames = dialog.selectedFiles();
  try {
    SiteListLoader::writeSiteList(sitesVisited_, filenames.first());
  } catch (const std::exception &) {
    QMessageBox::critical(this, tr("Failed to save file"),
                          tr("Could not save %1").arg(filenames.first()));
  }
}
void MainWindow::tabChanged(int index) {
  if (index < 0) {
    return;
  }
  const auto viewMode =
      widgets_.tabBar->tabData(index).value<FnSiteWidget::ViewMode>();
  widgets_.miraMap->setViewMode(viewMode);
}
