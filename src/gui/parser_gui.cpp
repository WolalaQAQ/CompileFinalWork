//
// Created by Adminstrator on 24-6-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_parser_gui.h" resolved

#include "gui/parser_gui.h"
#include "ui_parser_gui.h"

#include <QFileDialog>
#include <QListWidget>

#include "gui/dialog.h"

ParserGUI::ParserGUI(MainWindow *main_window, QWidget *parent) : main_window_(main_window),
                                                                 QWidget(parent),
                                                                 ui(new Ui::ParserGUI) {
    ui->setupUi(this);

    connect(main_window_, &MainWindow::sendCode, this, [this](const QString &code) {
        code_qstring_ = code;
    });
    connect(ui->inputPushButton, &QPushButton::clicked, this, &ParserGUI::setGrammarFilePath);
    connect(ui->startPushButton, &QPushButton::clicked, this, &ParserGUI::startParse);
}

ParserGUI::~ParserGUI() {
    delete ui;
}

void ParserGUI::setGrammarFilePath() {
    grammar_file_path_ = QFileDialog::getOpenFileName(this, tr("Open Grammar File"), "", tr("Grammar Files (*.txt)"));
    if (grammar_file_path_.isEmpty()) {
        dialog::showDialog("No file selected.", QMessageBox::Critical);
        return;
    }
    ui->inputLineEdit->setText(grammar_file_path_);
}

void ParserGUI::startParse() {
    if (grammar_file_path_.isEmpty()) {
        dialog::showDialog("Please select grammar file.", QMessageBox::Critical);
        return;
    }

    grammar_.initGrammar(grammar_file_path_.toStdString());
    grammar_.compute();
    auto parse_table = grammar_.getParseTable();

    scanner_.scan(code_qstring_.toStdString());
    auto answer_tokens = scanner_.getAnswerTokens();

    auto parse_result = parser_.parse(parse_table, answer_tokens);
    if (parse_result) {
        dialog::showDialog("Parse successfully.", QMessageBox::Information);
    } else {
        dialog::showDialog("Parse failed.", QMessageBox::Critical);
    }

    auto parse_history = parser_.getHistory();
    // 填充分析表的可视化
    for (const auto &history : parse_history) {
        auto *item = new QListWidgetItem(ui->listWidget);
        item->setText(QString::fromStdString(history));
    }
}
