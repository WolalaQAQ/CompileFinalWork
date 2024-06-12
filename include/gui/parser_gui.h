//
// Created by Adminstrator on 24-6-12.
//

#ifndef COMPILEFINALWORK_PARSER_GUI_H
#define COMPILEFINALWORK_PARSER_GUI_H

#include <QWidget>

#include "mainwindow.h"
#include "scanner.h"
#include "grammar.h"
#include "parser.h"

class MainWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
    class ParserGUI;
}
QT_END_NAMESPACE

class ParserGUI : public QWidget {
    Q_OBJECT

public:
    explicit ParserGUI(MainWindow *main_window, QWidget *parent = nullptr);

    ~ParserGUI() override;

private:
    Ui::ParserGUI *ui;
    MainWindow *main_window_;

    QString code_qstring_;
    QString grammar_file_path_;

    Scanner scanner_;
    Grammar grammar_;
    Parser parser_;


private slots:
    void setGrammarFilePath();
    void startParse();

};


#endif//COMPILEFINALWORK_PARSER_GUI_H
