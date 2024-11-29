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
#include <QHeaderView>
#include <QMenuBar>
#include <QMessageBox>
#include <QTabBar>

#include "FnSite.h"
#include "InventoryLoader.h"
#include "SiteListLoader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), inventoryModel_(new InventoryModel(this)) {
  initUi();
}

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
  menuFile->addAction(actions.fileImportInventory);
  menuFile->addAction(actions.fileExportInventory);
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

  // Config pane
  auto configLayout = new QVBoxLayout();
  layout->addLayout(configLayout);

  // Inventory
  widgets_.inventoryTable = new QTableView(central);
  configLayout->addWidget(widgets_.inventoryTable);
  widgets_.inventoryTable->setModel(inventoryModel_);
  widgets_.inventoryTable->verticalHeader()->hide();
  widgets_.inventoryTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  widgets_.inventoryTable->setSizePolicy(QSizePolicy::MinimumExpanding,
                                         QSizePolicy::MinimumExpanding);
  widgets_.inventoryTable->resizeColumnsToContents();

  // Run Options
  widgets_.runOptions = new RunOptionsWidget(central);
  configLayout->addWidget(widgets_.runOptions);
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
  // Import Inventory
  actions.fileImportInventory = new QAction(QIcon::fromTheme("document-import"),
                                            "Import Inventory", this);
  connect(actions.fileImportInventory, &QAction::triggered, this,
          &MainWindow::fileImportInventory);
  // Export Inventory
  actions.fileExportInventory = new QAction(QIcon::fromTheme("document-export"),
                                            "Export Inventory", this);
  connect(actions.fileExportInventory, &QAction::triggered, this,
          &MainWindow::fileExportInventory);
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

void MainWindow::fileImportInventory() {
  QFileDialog dialog(this, tr("Import Inventory..."));
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setNameFilter(tr("Inventory (*.csv)"));
  if (dialog.exec() != QDialog::Accepted) {
    return;
  }
  const auto filenames = dialog.selectedFiles();
  try {
    const auto newInventory = InventoryLoader::readInventory(filenames.first());
    inventoryModel_->setProbeInventory(newInventory);
  } catch (const std::exception &) {
    QMessageBox::critical(this, tr("Failed to open file"),
                          tr("Could not open %1").arg(filenames.first()));
  }
}

void MainWindow::fileExportInventory() {
  QFileDialog dialog(this, tr("Export Inventory..."));
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setNameFilter(tr("Inventory (*.csv)"));
  if (dialog.exec() != QDialog::Accepted) {
    return;
  }
  const auto filenames = dialog.selectedFiles();
  try {
    InventoryLoader::writeInventory(inventoryModel_->probeInventory(),
                                    filenames.first());
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
