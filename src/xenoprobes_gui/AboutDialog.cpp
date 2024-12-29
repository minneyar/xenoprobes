/**
 * @file AboutDialog.cpp
 *
 * @author Dan Keenan
 * @date 12/29/2024
 * @copyright GNU GPLv3
 */

#include "AboutDialog.h"
#include "xenoprobes_config.h"
#include <QApplication>
#include <QDateTime>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
  auto *layout = new QVBoxLayout(this);

  // Icon
  auto *iconLabel = new QLabel(this);
  iconLabel->setPixmap(QPixmap(":/dataprobe.png"));
  layout->addWidget(iconLabel, 0, Qt::AlignCenter);

  // Title
  auto *titleLabel = new QLabel(
      QString("<b><big>%1</big></b>").arg(qApp->applicationDisplayName()),
      this);
  layout->addWidget(titleLabel, 0, Qt::AlignCenter);

  // Version
  auto *versionLabel = new QLabel(qApp->applicationVersion(), this);
  layout->addWidget(versionLabel, 0, Qt::AlignCenter);

  // Build time
  const auto buildTime =
      QDateTime::fromSecsSinceEpoch(config::kProjectBuildTimestamp);
  auto *buildtimeLabel = new QLabel(
      tr("Built %1").arg(QLocale().toString(buildTime, QLocale::ShortFormat)),
      this);
  layout->addWidget(buildtimeLabel, 0, Qt::AlignCenter);

  // Github
  auto *websiteLabel = new QLabel(
      QString("<a href=\"%1\">%1</a>").arg(config::kProjectHomepageUrl), this);
  websiteLabel->setOpenExternalLinks(true);
  layout->addWidget(websiteLabel, 0, Qt::AlignCenter);

  // Authors
  auto *authorsTitle = new QLabel(tr("Authors:"), this);
  layout->addWidget(authorsTitle, 0, Qt::AlignCenter);
  auto *authorsLabel = new QLabel(QString::fromUtf8(config::kAuthors), this);
  authorsLabel->setTextFormat(Qt::MarkdownText);
  authorsLabel->setOpenExternalLinks(true);
  layout->addWidget(authorsLabel, 0, Qt::AlignCenter);

  // About Qt
  auto *aboutQtLabel =
      new QLabel(tr("<a href=\"aboutqt\">About Qt...</a>"), this);
  connect(aboutQtLabel, &QLabel::linkActivated, qApp, &QApplication::aboutQt);
  layout->addWidget(aboutQtLabel, 0, Qt::AlignCenter);

  // Close button
  auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &AboutDialog::close);
  layout->addWidget(buttonBox);
}
