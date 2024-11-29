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
#include <QJsonDocument>
#include <QMenuBar>
#include <QMessageBox>
#include <QTabBar>

#include "FnSite.h"
#include "FnSiteWidget.h"
#include "InventoryLoader.h"
#include "SiteListLoader.h"

#include <QJsonObject>

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
  menuFile->addAction(actions.fileOpen);
  menuFile->addAction(actions.fileSave);
  menuFile->addAction(actions.fileSaveAs);
  menuFile->addSeparator();
  menuFile->addAction(actions.fileImportSites);
  menuFile->addAction(actions.fileExportSites);
  menuFile->addAction(actions.fileImportInventory);
  menuFile->addAction(actions.fileExportInventory);
  menuFile->addSeparator();
  menuFile->addAction(actions.fileExit);

  // Map
  auto mapLayout = new QVBoxLayout();
  layout->addLayout(mapLayout);
  widgets_.miraMap = new MiraMap(central);
  mapLayout->addWidget(widgets_.miraMap);
  // Start with all sites enabled.
  FnSite::IdList sitesVisited(FnSite::kAllSites.keyBegin(),
                              FnSite::kAllSites.keyEnd());
  widgets_.miraMap->setSitesVisited(sitesVisited);
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
  // Open
  actions.fileOpen =
      new QAction(QIcon::fromTheme("document-open"), tr("Open"), this);
  actions.fileOpen->setShortcut(QKeySequence::Open);
  connect(actions.fileOpen, &QAction::triggered, this, &MainWindow::fileOpen);
  // Save
  actions.fileSave =
      new QAction(QIcon::fromTheme("document-save"), tr("Save"), this);
  actions.fileSave->setShortcut(QKeySequence::Save);
  connect(actions.fileSave, &QAction::triggered, this, &MainWindow::fileSave);
  // Save As
  actions.fileSaveAs =
      new QAction(QIcon::fromTheme("document-save-as"), tr("Save As"), this);
  actions.fileSaveAs->setShortcut(QKeySequence::SaveAs);
  connect(actions.fileSaveAs, &QAction::triggered, this,
          &MainWindow::fileSaveAs);
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

void MainWindow::fileOpen() {
  QFileDialog dialog(this, tr("Open Simulation..."));
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setNameFilter(tr("Simulations (*.json)"));
  if (dialog.exec() != QDialog::Accepted) {
    return;
  }
  const auto filenames = dialog.selectedFiles();
  try {
    QFile file(filenames.at(0));
    if (!file.open(QIODevice::ReadOnly)) {
      QMessageBox::critical(this, tr("Error opening file"),
                            tr("Could not open %1").arg(filenames.first()));
      return;
    }
    const QByteArray data = file.readAll();
    const auto json = QJsonDocument::fromJson(data);
    if (json.isNull() || !json.isObject()) {
      QMessageBox::critical(this, tr("Error opening file"),
                            tr("%1 is not valid.").arg(filenames.first()));
      return;
    }
    const auto jsonObj = json.object();
    if (!jsonObj.contains("sites") || !jsonObj.contains("inventory") ||
        !jsonObj.contains("probeMap") || !jsonObj.contains("options")) {
      QMessageBox::critical(this, tr("Error opening file"),
                            tr("%1 is not valid.").arg(filenames.first()));
      return;
    }
    const auto sites = SiteListLoader::readSiteListFromJson(jsonObj["sites"]);
    const auto probeMap = MiraMap::siteProbesFromJson(jsonObj["probeMap"]);
    const auto inventory =
        InventoryLoader::readInventoryFromJson(jsonObj["inventory"]);
    const auto options = RunOptionsWidget::optionsFromJson(jsonObj["options"]);

    // Defer saving until everything is loaded in case of errors.
    widgets_.miraMap->setSitesVisited(sites);
    widgets_.miraMap->setSiteProbeMap(probeMap);
    inventoryModel_->setProbeInventory(inventory);
    widgets_.runOptions->setOptions(options);
  } catch (std::runtime_error &) {
    QMessageBox::critical(this, tr("Error opening file"),
                          tr("%1 is not valid.").arg(filenames.first()));
  }
}
void MainWindow::fileSave() {
  if (windowFilePath().isEmpty()) {
    fileSaveAs();
  } else {
    saveToPath(windowFilePath());
  }
}
void MainWindow::fileSaveAs() {
  QFileDialog dialog(this, tr("Export Simulation..."));
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setNameFilter(tr("Simulations (*.json)"));
  if (dialog.exec() != QDialog::Accepted) {
    return;
  }
  const auto filenames = dialog.selectedFiles();
  saveToPath(filenames.at(0));
}

void MainWindow::saveToPath(const QString &path) {
  QFile file(path);
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, tr("Error saving file"),
                          tr("The file could not be opened for writing."));
    return;
  }
  QJsonObject json;
  json["sites"] =
      SiteListLoader::writeSiteListToJson(widgets_.miraMap->sitesVisited());
  json["probeMap"] =
      MiraMap::siteProbesToJson(widgets_.miraMap->siteProbeMap());
  json["inventory"] =
      InventoryLoader::writeInventoryToJson(inventoryModel_->probeInventory());
  json["options"] =
      RunOptionsWidget::optionsToJson(widgets_.runOptions->options());
  file.write(QJsonDocument(json).toJson());
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
    const auto newIds = SiteListLoader::readSiteListFromFile(filenames.first());
    widgets_.miraMap->setSitesVisited(newIds);
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
    SiteListLoader::writeSiteListToFile(widgets_.miraMap->sitesVisited(),
                                        filenames.first());
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
    const auto newInventory =
        InventoryLoader::readInventoryFromFile(filenames.first());
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
    InventoryLoader::writeInventoryToFile(inventoryModel_->probeInventory(),
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
