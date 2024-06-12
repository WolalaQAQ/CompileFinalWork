//
// Created by Adminstrator on 24-6-13.
//

#ifndef COMPILEFINALWORK_IR_GENERATOR_GUI_H
#define COMPILEFINALWORK_IR_GENERATOR_GUI_H

#include <QWidget>
#include <QString>

#include "mainwindow.h"

#include "ir_generator.h"

class MainWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
    class IRGeneratorGUI;
}
QT_END_NAMESPACE

class IRGeneratorGUI : public QWidget {
    Q_OBJECT

public:
    explicit IRGeneratorGUI(MainWindow *main_window, QWidget *parent = nullptr);
    ~IRGeneratorGUI() override;

private:
    Ui::IRGeneratorGUI *ui;
    MainWindow *main_window_;
    Scanner scanner_;
    IRGenerator ir_generator_;

    QString code_qstring_;

private slots:
    void generateIR();

};


#endif//COMPILEFINALWORK_IR_GENERATOR_GUI_H
