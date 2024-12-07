/**
 * @file MainWindow.cpp
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#include "MainWindow.h"
#include <QCloseEvent>
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
#include "settings.h"

#include <QJsonObject>
#include <QPushButton>
#include <qguiapplication.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), inventoryModel_(new InventoryModel(this)) {
  initUi();
  connect(inventoryModel_, &InventoryModel::dataChanged, this,
          &MainWindow::dataChanged);
}

void MainWindow::initUi() {
  if (!restoreGeometry(settings::getMainWindowGeometry())) {
    resize(1024, 768);
  }
  setWindowTitle(QString("%1[*]").arg(qApp->applicationDisplayName()));

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
  connect(widgets_.miraMap, &MiraMap::sitesVisitedChanged, this,
          &MainWindow::dataChanged);
  connect(widgets_.miraMap, &MiraMap::siteProbeMapChanged, this,
          &MainWindow::dataChanged);

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
  connect(widgets_.runOptions, &RunOptionsWidget::settingsChanged, this,
          &MainWindow::dataChanged);

  // Solve button
  auto *solveBtn = new QPushButton(tr("Solve"), central);
  configLayout->addWidget(solveBtn);
  connect(solveBtn, &QPushButton::clicked, this, &MainWindow::solve);
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

void MainWindow::updateWindowTitle() {
  if (!windowFilePath().isEmpty()) {
    setWindowTitle(QString("%1[*]").arg(windowFilePath()));
  } else if (isWindowModified()) {
    setWindowTitle(tr("Unsaved file[*]"));
  } else {
    setWindowTitle({});
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (!safeToCloseFile()) {
    event->ignore();
    return;
  }
  settings::setMainWindowGeometry(saveGeometry());
  event->accept();
}

bool MainWindow::safeToCloseFile() {
  if (isWindowModified()) {
    QMessageBox dialog(this);
    dialog.setIcon(QMessageBox::Question);
    dialog.setText(tr("This layout has been modified."));
    dialog.setInformativeText(tr("Would you like to save your changes?"));
    dialog.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                              QMessageBox::Cancel);
    dialog.setDefaultButton(QMessageBox::Save);
    const auto ret = dialog.exec();
    if (ret == QMessageBox::Save) {
      // Save, then close.
      fileSave();
      if (isWindowModified()) {
        // User cancelled save box.
        return false;
      }
    } else if (ret == QMessageBox::Cancel) {
      // Don't close.
      return false;
    }
  }
  // Either no need to save or discard changes and close.

  return true;
}

void MainWindow::fileOpen() {
  if (!safeToCloseFile()) {
    return;
  }

  QFileDialog dialog(this, tr("Open Simulation..."));
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setNameFilter(tr("Simulations (*.json)"));
  dialog.setDirectory(settings::getLastFileDialogPath());
  if (dialog.exec() != QDialog::Accepted) {
    return;
  }
  const auto filenames = dialog.selectedFiles();
  const QFileInfo fileInfo(filenames.at(0));
  try {
    QFile file(fileInfo.filePath());
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

    settings::setLastFileDialogPath(fileInfo.absoluteDir().path());
    setWindowFilePath(fileInfo.filePath());
    setWindowModified(false);
    updateWindowTitle();
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
  if (!windowFilePath().isEmpty()) {
    dialog.selectFile(windowFilePath());
  } else {
    dialog.setDirectory(settings::getLastFileDialogPath());
  }
  if (dialog.exec() != QDialog::Accepted) {
    return;
  }
  const auto filenames = dialog.selectedFiles();
  const QFileInfo fileInfo(filenames.at(0));
  saveToPath(fileInfo.filePath());
  settings::setLastFileDialogPath(fileInfo.absoluteDir().path());
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
  setWindowFilePath(path);
  setWindowModified(false);
  updateWindowTitle();
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
    dataChanged();
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
    dataChanged();
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

void MainWindow::dataChanged() {
  setWindowModified(true);
  updateWindowTitle();
}

void MainWindow::solve() {
  progressDialog_ = new QProgressDialog(this);
  progressDialog_->setWindowTitle(tr("Calculating..."));
  progressDialog_->setCancelButtonText(tr("Cancel"));
  progressDialog_->setMinimumDuration(0);
  progressDialog_->setMinimum(0);
  progressDialog_->setMaximum(widgets_.runOptions->options().iterations);
  // Can't use autoreset because progress is reported *before* the calculation,
  // not after.
  progressDialog_->setAutoReset(false);
  solverRunner_ = new SolverRunner(
      widgets_.miraMap->sitesVisited(), widgets_.miraMap->siteProbeMap(),
      inventoryModel_->probeInventory(), widgets_.runOptions->options(), this);
  connect(solverRunner_, &SolverRunner::progress, this, &MainWindow::progress);
  connect(progressDialog_, &QProgressDialog::canceled, solverRunner_,
          &SolverRunner::requestInterruption);
  connect(solverRunner_, &SolverRunner::finished, this, &MainWindow::solved);
  solverRunner_->start();
}

void MainWindow::progress(unsigned long iter, double bestScore,
                          double worstScore, unsigned long killed) {
  if (!progressDialog_) {
    return;
  }
  progressDialog_->setValue(iter);
  progressDialog_->setLabelText(tr("Best: %1\nWorst: %2\nKilled: %3")
                                    .arg(std::lround(bestScore))
                                    .arg(std::lround(worstScore))
                                    .arg(killed));
}

void MainWindow::solved(unsigned int mining, unsigned int revenue,
                        unsigned int storage, QStringList ores,
                        MiraMap::SiteProbeMap siteProbeMap) {
  progressDialog_->close();
  progressDialog_->deleteLater();
  progressDialog_ = nullptr;

  widgets_.miraMap->setSiteProbeMap(siteProbeMap);

  QMessageBox resultDialog(this);
  resultDialog.setText(tr("Completed solving!"));
  QStringList oresList;
  for (const auto &ore : ores) {
    oresList.append(QString("<li>%1</li>").arg(ore));
  }
  // @formatter:off
  // clang-format off
  resultDialog.setInformativeText(
      tr(
          "<table>"
            "<tr><th align=\"right\">Production</th><td align=\"left\">%1</td></tr>"
            "<tr><th align=\"right\">Revenue</th><td align=\"left\">%2</td></tr>"
            "<tr><th align=\"right\">Storage</th><td align=\"left\">%3</td></tr>"
            "<tr><th align=\"right\">Ores</th><td align=\"left\"><ul>%4</ul></td></tr>"
          "</table>"
          )
          .arg(mining)
          .arg(revenue)
          .arg(storage)
          .arg(oresList.join("")));
  // @formatter:on
  // clang-format on
  resultDialog.exec();
  solverRunner_->deleteLater();
  solverRunner_ = nullptr;
  dataChanged();
}
