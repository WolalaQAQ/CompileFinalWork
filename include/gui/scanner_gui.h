//
// Created by wolala on 24-6-12.
//

#ifndef COMPILEFINALWORK_SRC_GUI_SCANNER_GUI_H
#define COMPILEFINALWORK_SRC_GUI_SCANNER_GUI_H

#include <QWidget>
#include <QString>

#include "scanner.h"
#include "gui/mainwindow.h"

class MainWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class ScannerGUI; }
QT_END_NAMESPACE

class ScannerGUI : public QWidget {
Q_OBJECT

public:
  explicit ScannerGUI(MainWindow *main_window, QWidget *parent = nullptr);
  ~ScannerGUI() override;

private:
  Ui::ScannerGUI *ui;
  MainWindow* main_window_;
  Scanner scanner_;
  QString code_qstring_;

private slots:
  void scanCode();

};

#endif //COMPILEFINALWORK_SRC_GUI_SCANNER_GUI_H
