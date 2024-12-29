/**
 * @file MainWindow.cpp
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#include "MainWindow.h"
#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QTabBar>

#include "FnSiteWidget.h"
#include "InventoryLoader.h"
#include "SiteListLoader.h"
#include "settings.h"
#include "themeIcon.h"

#include <probeoptimizer/site.h>

#include <ranges>
#include <spdlog/spdlog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      inventoryModel_(new InventoryModel(&probeOptimizer_, this)) {
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
  menuFile->addMenu(actions.fileRecent);
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
  // Start with all sites enabled.
  ProbeOptimizer::SiteList sites;
  for (const auto site : Site::ALL | std::views::values) {
    sites.insert(site);
  }
  probeOptimizer_.loadSites(sites);
  widgets_.miraMap = new MiraMap(&probeOptimizer_, central);
  mapLayout->addWidget(widgets_.miraMap);

  widgets_.miraMap->show();
  connect(widgets_.miraMap, &MiraMap::sitesVisitedChanged, this,
          &MainWindow::dataChanged);
  connect(widgets_.miraMap, &MiraMap::siteProbeMapChanged, this,
          &MainWindow::dataChanged);
  connect(widgets_.miraMap, &MiraMap::sitesVisitedChanged, this,
          &MainWindow::probeMapChanged);
  connect(widgets_.miraMap, &MiraMap::siteProbeMapChanged, this,
          &MainWindow::probeMapChanged);

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
  widgets_.inventoryTable->setSizePolicy(QSizePolicy::MinimumExpanding,
                                         QSizePolicy::MinimumExpanding);
  widgets_.inventoryTable->resizeColumnsToContents();

  // Run Options
  auto *optionsScrollArea = new QScrollArea(central);
  widgets_.runOptions = new RunOptionsWidget(central);
  optionsScrollArea->setWidget(widgets_.runOptions);
  configLayout->addWidget(optionsScrollArea);
  connect(widgets_.runOptions, &RunOptionsWidget::settingsChanged, this,
          &MainWindow::dataChanged);

  // Current solution
  auto *solutionScrollArea = new QScrollArea(central);
  widgets_.solutionWidget = new SolutionWidget(central);
  widgets_.solutionWidget->setSolution(probeOptimizer_.solution().getSetup());
  solutionScrollArea->setWidget(widgets_.solutionWidget);
  solutionScrollArea->setWidgetResizable(true);
  configLayout->addWidget(solutionScrollArea);

  // Solve button
  auto *solveBtn = new QPushButton(tr("Solve"), central);
  configLayout->addWidget(solveBtn);
  connect(solveBtn, &QPushButton::clicked, this, &MainWindow::solve);
}

void MainWindow::initActions() {
  // File
  // Open
  actions.fileOpen =
      new QAction(qIconFromTheme(ThemeIcon::DocumentOpen), tr("&Open"), this);
  actions.fileOpen->setShortcut(QKeySequence::Open);
  connect(actions.fileOpen, &QAction::triggered, this, &MainWindow::fileOpen);
  // Recent
  actions.fileRecent = new QMenu(tr("Recent Documents"), this);
  actions.fileRecent->setIcon(qIconFromTheme(ThemeIcon::DocumentOpenRecent));
  updateRecentDocuments();
  // Save
  actions.fileSave =
      new QAction(qIconFromTheme(ThemeIcon::DocumentSave), tr("&Save"), this);
  actions.fileSave->setShortcut(QKeySequence::Save);
  connect(actions.fileSave, &QAction::triggered, this, &MainWindow::fileSave);
  // Save As
  actions.fileSaveAs = new QAction(qIconFromTheme(ThemeIcon::DocumentSaveAs),
                                   tr("Save &As"), this);
  actions.fileSaveAs->setShortcut(QKeySequence::SaveAs);
  connect(actions.fileSaveAs, &QAction::triggered, this,
          &MainWindow::fileSaveAs);
  // Import Sites
  actions.fileImportSites = new QAction("Import Sites", this);
  connect(actions.fileImportSites, &QAction::triggered, this,
          &MainWindow::fileImportSites);
  // Export Sites
  actions.fileExportSites = new QAction("Export Sites", this);
  connect(actions.fileExportSites, &QAction::triggered, this,
          &MainWindow::fileExportSites);
  // Import Inventory
  actions.fileImportInventory = new QAction("Import Inventory", this);
  connect(actions.fileImportInventory, &QAction::triggered, this,
          &MainWindow::fileImportInventory);
  // Export Inventory
  actions.fileExportInventory = new QAction("Export Inventory", this);
  connect(actions.fileExportInventory, &QAction::triggered, this,
          &MainWindow::fileExportInventory);
  // Exit
  actions.fileExit =
      new QAction(qIconFromTheme(ThemeIcon::ApplicationExit), "E&xit", this);
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

void MainWindow::addRecentDocument(const QString &path) {
  auto recentPaths = settings::getRecentDocuments();
  recentPaths.prepend(path);
  settings::setRecentDocuments(recentPaths);
  updateRecentDocuments();
}

void MainWindow::updateRecentDocuments() {
  auto recentPaths = settings::getRecentDocuments();
  recentPaths.removeDuplicates();
  recentPaths.removeIf([](const QString &path) {
    QFileInfo fileInfo(path);
    return !fileInfo.exists();
  });
  recentPaths.resize(
      std::min(recentPaths.size(), settings::getMaxRecentDocuments()));
  actions.fileRecent->setEnabled(!recentPaths.empty());
  actions.fileRecent->clear();
  for (const auto &recentPath : recentPaths) {
    auto *actRecentPath = new QAction(recentPath, actions.fileRecent);
    connect(actRecentPath, &QAction::triggered, [this, recentPath]() {
      if (safeToCloseFile()) {
        openFromPath(recentPath);
      }
    });
    actions.fileRecent->addAction(actRecentPath);
  }
  settings::setRecentDocuments(recentPaths);
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
  openFromPath(fileInfo.filePath());
  settings::setLastFileDialogPath(fileInfo.absoluteDir().path());
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

void MainWindow::openFromPath(const QString &path) {
  try {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
      QMessageBox::critical(this, tr("Error opening file"),
                            tr("Could not open %1").arg(path));
      return;
    }
    const QByteArray data = file.readAll();
    const auto json = QJsonDocument::fromJson(data);
    if (json.isNull() || !json.isObject()) {
      QMessageBox::critical(this, tr("Error opening file"),
                            tr("%1 is not valid.").arg(path));
      return;
    }
    const auto jsonObj = json.object();
    if (!jsonObj.contains("sites") || !jsonObj.contains("inventory") ||
        !jsonObj.contains("probeMap") || !jsonObj.contains("options")) {
      QMessageBox::critical(this, tr("Error opening file"),
                            tr("%1 is not valid.").arg(path));
      return;
    }

    ProbeOptimizer newProbeOptimizer;
    const auto sites = SiteListLoader::readSiteListFromJson(jsonObj["sites"]);
    newProbeOptimizer.loadSites(sites);
    const auto inventory =
        InventoryLoader::readInventoryFromJson(jsonObj["inventory"]);
    newProbeOptimizer.loadInventory(inventory);
    const auto probeMap =
        SiteListLoader::readSiteProbesFromJson(jsonObj["probeMap"]);
    newProbeOptimizer.loadSetup(probeMap);
    const auto options = RunOptionsWidget::optionsFromJson(jsonObj["options"]);

    // Defer saving until everything is loaded in case of errors.
    probeOptimizer_ = newProbeOptimizer;
    widgets_.miraMap->setProbeOptimizer(&probeOptimizer_);
    inventoryModel_->setProbeOptimizer(&probeOptimizer_);
    widgets_.runOptions->setOptions(options);
    widgets_.solutionWidget->setSolution(probeOptimizer_.solution().getSetup());

    setWindowFilePath(path);
    setWindowModified(false);
    updateWindowTitle();
    addRecentDocument(path);
  } catch (std::runtime_error &) {
    QMessageBox::critical(this, tr("Error opening file"),
                          tr("%1 is not valid.").arg(path));
  }
}

void MainWindow::saveToPath(const QString &path) {
  QJsonObject json;
  json["sites"] =
      SiteListLoader::writeSiteListToJson(probeOptimizer_.getSites());
  json["probeMap"] = SiteListLoader::writeSiteProbesToJson(
      probeOptimizer_.solution().getSetup());
  json["inventory"] =
      InventoryLoader::writeInventoryToJson(probeOptimizer_.getInventory());
  json["options"] =
      RunOptionsWidget::optionsToJson(widgets_.runOptions->options());

  QFile file(path);
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, tr("Error saving file"),
                          tr("The file could not be opened for writing."));
    return;
  }
  file.write(QJsonDocument(json).toJson());
  setWindowFilePath(path);
  setWindowModified(false);
  updateWindowTitle();
  addRecentDocument(path);
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
    const auto siteList =
        SiteListLoader::readSiteListFromFile(filenames.first());
    probeOptimizer_.loadSites(siteList);
    widgets_.miraMap->setProbeOptimizer(&probeOptimizer_);
    dataChanged();
    probeMapChanged();
  } catch (const std::exception &e) {
    spdlog::error(e.what());
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
    SiteListLoader::writeSiteListToFile(probeOptimizer_.getSites(),
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
    probeOptimizer_.loadInventory(filenames.first().toStdString());
    inventoryModel_->setProbeOptimizer(&probeOptimizer_);
    dataChanged();
    probeMapChanged();
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
    InventoryLoader::writeInventoryToFile(probeOptimizer_.getInventory(),
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

void MainWindow::probeMapChanged() {
  inventoryModel_->setProbeOptimizer(&probeOptimizer_);
  widgets_.solutionWidget->setSolution(probeOptimizer_.solution().getSetup());
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
  progressDialog_->setModal(true);
  solverRunner_ =
      new SolverRunner(&probeOptimizer_, widgets_.runOptions->options(), this);
  connect(solverRunner_, &SolverRunner::progress, this, &MainWindow::progress);
  connect(progressDialog_, &QProgressDialog::canceled, solverRunner_,
          &SolverRunner::requestInterruption);
  connect(solverRunner_, &SolverRunner::finished, this, &MainWindow::solved);
  solverStopwatch_.start();
  solverRunner_->start();
}

void MainWindow::progress(unsigned long iter, double bestScore,
                          double worstScore, unsigned long killed) {
  if (!progressDialog_) {
    return;
  }
  progressDialog_->setValue(iter);
  const long long msElapsed = solverStopwatch_.elapsed();
  const long long totalMsRequired =
      ((solverStopwatch_.elapsed() / iter) * widgets_.runOptions->iterations());
  const auto hoursRequired = totalMsRequired / 1000 / 60 / 60;
  const auto minutesRequired = (totalMsRequired / 1000 / 60) % 60;
  const auto secondsRequired = totalMsRequired / 1000 % 60;
  const auto totalSecsRemaining =
      std::max(static_cast<long long>(0), totalMsRequired - msElapsed) / 1000;
  const auto hoursRemaining = totalSecsRemaining / 60 / 60;
  const auto minutesRemaining = (totalSecsRemaining / 60) % 60;
  const auto secondsRemaining = totalSecsRemaining % 60;
  progressDialog_->setLabelText(tr("%1/%2\nBest: %3\nWorst: %4\nKilled: "
                                   "%5\n\nRequired: %6:%7:%8\nRemaining: "
                                   "%9:%10:%11")
                                    .arg(iter)
                                    .arg(widgets_.runOptions->iterations())
                                    .arg(std::lround(bestScore))
                                    .arg(std::lround(worstScore))
                                    .arg(killed)
                                    .arg(hoursRequired)
                                    .arg(minutesRequired, 2, 10, QChar('0'))
                                    .arg(secondsRequired, 2, 10, QChar('0'))
                                    .arg(hoursRemaining)
                                    .arg(minutesRemaining, 2, 10, QChar('0'))
                                    .arg(secondsRemaining, 2, 10, QChar('0')));
}

void MainWindow::solved(ProbeArrangement probeArrangement) {
  progressDialog_->close();
  progressDialog_->deleteLater();
  progressDialog_ = nullptr;
  solverStopwatch_.invalidate();

  probeOptimizer_.loadSetup(probeArrangement);
  widgets_.miraMap->setProbeOptimizer(&probeOptimizer_);
  widgets_.solutionWidget->setSolution(probeArrangement);

  QStringList ores;
  for (const auto &ore : probeArrangement.getOres()) {
    ores.push_back(QString::fromStdString(ore));
  }

  QMessageBox resultDialog(this);
  resultDialog.setText(tr("Completed solving!"));
  // @formatter:off
  // clang-format off
  resultDialog.setInformativeText(
      tr(
          "<table>"
            "<tr><th align=\"right\">Production</th><td align=\"left\">%1</td></tr>"
            "<tr><th align=\"right\">Revenue</th><td align=\"left\">%2</td></tr>"
            "<tr><th align=\"right\">Storage</th><td align=\"left\">%3</td></tr>"
            "<tr><th align=\"right\">Ores</th><td align=\"left\">%4</td></tr>"
          "</table>"
          )
          .arg(probeArrangement.getTotalProduction())
          .arg(probeArrangement.getTotalRevenue())
          .arg(probeArrangement.getTotalStorage())
          .arg(ores.join("<br>")));
  // @formatter:on
  // clang-format on
  resultDialog.exec();
  solverRunner_->deleteLater();
  solverRunner_ = nullptr;
  dataChanged();
}
