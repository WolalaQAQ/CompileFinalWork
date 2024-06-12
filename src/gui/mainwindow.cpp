//
// Created by wolala on 24-6-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "gui/mainwindow.h"
#include "ui_mainwindow.h"

#include "gui/dialogs.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    scanner_gui_ = new ScannerGUI(this);
    scanner_gui_->hide();

    grammar_gui_ = new GrammarGUI();
    grammar_gui_->hide();

    parser_gui_ = new ParserGUI(this);
    parser_gui_->hide();

    connect(ui->readCodeAction, &QAction::triggered, this, &MainWindow::readCode);
    connect(ui->codeTextEdit, &QTextEdit::textChanged, this, &MainWindow::changeCode);
    connect(ui->scannerPushButton, &QPushButton::clicked, scanner_gui_, [this] {
        if (code_qstring_.isEmpty()) {
            dialogs::showErrorDialog("No code to scan.");
            return;
        }
        scanner_gui_->show();
        emit sendCode(code_qstring_);
    });
    connect(ui->grammarPushButton, &QPushButton::clicked, grammar_gui_, [this] {
        grammar_gui_->show();
    });
    connect(ui->parserPushButton, &QPushButton::clicked, parser_gui_, [this] {
        if (code_qstring_.isEmpty()) {
            dialogs::showErrorDialog("No code to parse.");
            return;
        }
        parser_gui_->show();
        emit sendCode(code_qstring_);
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::readCode() {
    QString codePath = QFileDialog::getOpenFileName(this, tr("Open Code File"), "", tr("C Source Files (*.c)"));
    if (codePath.isEmpty()) {
        dialogs::showErrorDialog("No file selected.");
        return;
    }

    QFile codeFile(codePath);
    if (!codeFile.open(QIODevice::ReadOnly)) {
        dialogs::showErrorDialog("Failed to open file.");
        return;
    }

    code_qstring_ = codeFile.readAll();
    //	qDebug() << code_qstring_;
    ui->codeTextEdit->setPlainText(code_qstring_);
}

void MainWindow::changeCode() {
    code_qstring_ = ui->codeTextEdit->toPlainText();
    //	qDebug() << code_qstring_;
}
