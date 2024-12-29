/**
 * @file AboutDialog.h
 *
 * @author Dan Keenan
 * @date 12/29/2024
 * @copyright GNU GPLv3
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class AboutDialog : public QDialog {
  Q_OBJECT
public:
  explicit AboutDialog(QWidget *parent = nullptr);
};

#endif // ABOUTDIALOG_H
