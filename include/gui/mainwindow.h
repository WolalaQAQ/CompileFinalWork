//
// Created by wolala on 24-6-12.
//

#ifndef COMPILEFINALWORK_GUI_MAINWINDOW_H
#define COMPILEFINALWORK_GUI_MAINWINDOW_H

/* Qt Headers */
#include <QMainWindow>
#include <QString>

/* Standard Headers */
#include <memory>
#include <string>

/* Self-defined Headers */
#include "scanner_gui.h"
#include "grammar_gui.h"
#include "parser_gui.h"
#include "ir_generator_gui.h"

class ScannerGUI;
class GrammarGUI;
class ParserGUI;
class IRGeneratorGUI;

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    ScannerGUI *scanner_gui_ = nullptr;
    GrammarGUI *grammar_gui_ = nullptr;
    ParserGUI *parser_gui_ = nullptr;
    IRGeneratorGUI *ir_generator_gui_ = nullptr;

    QString code_qstring_;

private slots:
    void readCode();
    void changeCode();

signals:
    void sendCode(const QString &code);
};

#endif//COMPILEFINALWORK_GUI_MAINWINDOW_H
