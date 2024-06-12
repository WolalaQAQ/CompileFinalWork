//
// Created by Adminstrator on 24-6-12.
//

#ifndef COMPILEFINALWORK_GRAMMAR_GUI_H
#define COMPILEFINALWORK_GRAMMAR_GUI_H

#include <QWidget>
#include <QVector>

#include "grammar.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class GrammarGUI;
}
QT_END_NAMESPACE

class GrammarGUI : public QWidget {
    Q_OBJECT

public:
    explicit GrammarGUI(QWidget *parent = nullptr);

    ~GrammarGUI() override;

private:
    Ui::GrammarGUI *ui;
    Grammar grammar_;

    QString grammar_file_path_;
    QString sets_file_path_;

private slots:
    void setGrammarFilePath();
    void setSetsFilePath();
    void getSets();

};


#endif//COMPILEFINALWORK_GRAMMAR_GUI_H
