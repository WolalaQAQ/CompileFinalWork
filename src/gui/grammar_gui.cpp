//
// Created by Adminstrator on 24-6-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_grammar_gui.h" resolved

#include "gui/grammar_gui.h"
#include "ui_grammar_gui.h"

#include <QFileDialog>
#include <QTreeWidget>

#include "gui/dialogs.h"

GrammarGUI::GrammarGUI(QWidget *parent) : QWidget(parent), ui(new Ui::GrammarGUI) {
    ui->setupUi(this);

    connect(ui->inputPushButton, &QPushButton::clicked, this, &GrammarGUI::setGrammarFilePath);
    connect(ui->outputPushButton, &QPushButton::clicked, this, &GrammarGUI::setSetsFilePath);
    connect(ui->startPushButton, &QPushButton::clicked, this, &GrammarGUI::getSets);

}

GrammarGUI::~GrammarGUI() {
    delete ui;
}

void GrammarGUI::setGrammarFilePath() {
    grammar_file_path_ = QFileDialog::getOpenFileName(this, tr("Open Grammar File"), "", tr("Grammar Files (*.txt)"));
    if (grammar_file_path_.isEmpty()) {
        dialogs::showErrorDialog("No file selected.");
        return;
    }
    ui->inputLineEdit->setText(grammar_file_path_);
}

void GrammarGUI::setSetsFilePath() {
    sets_file_path_ = QFileDialog::getSaveFileName(this, tr("Save Sets File"), "", tr("Sets Files (*.txt)"));
    if (sets_file_path_.isEmpty()) {
        dialogs::showErrorDialog("No file selected.");
        return;
    }
    ui->outputLineEdit->setText(sets_file_path_);
}

void GrammarGUI::getSets() {
    if (grammar_file_path_.isEmpty() || sets_file_path_.isEmpty()) {
        dialogs::showErrorDialog("Please select grammar file and sets file.");
        return;
    }

    grammar_.initGrammar(grammar_file_path_.toStdString());
    grammar_.compute();
    grammar_.write_sets_to_file(sets_file_path_.toStdString());

    dialogs::showInfoDialog("Sets generated successfully.");

    // 先填充First集和Follow集的可视化
    auto first_set = grammar_.getFirstSet();
    auto follow_set = grammar_.getFollowSet();
    auto non_terminals = grammar_.getNonTerminals();

    ui->firstTreeWidget->clear();
    for (const auto &nt: non_terminals) {
        auto* root_item = new QTreeWidgetItem(ui->firstTreeWidget);
        auto first_set_str = QString::fromStdString(nt);
        root_item->setText(0, first_set_str);

        for (const auto &symbol: first_set[nt]) {
            auto* child_item = new QTreeWidgetItem(root_item);
            child_item->setText(1, QString::fromStdString(symbol));
        }
    }
    ui->firstTreeWidget->expandAll();

    ui->followTreeWidget->clear();
    for (const auto &nt: non_terminals) {
        auto* root_item = new QTreeWidgetItem(ui->followTreeWidget);
        auto follow_set_str = QString::fromStdString(nt);
        root_item->setText(0, follow_set_str);

        for (const auto &symbol: follow_set[nt]) {
            auto* child_item = new QTreeWidgetItem(root_item);
            child_item->setText(1, QString::fromStdString(symbol));
        }
    }
    ui->followTreeWidget->expandAll();

    // 再填充Select集的可视化
    auto select_set = grammar_.getSelectSet();

    ui->selectTreeWidget->clear();
    for (const auto &select: select_set) {
        auto* root_item = new QTreeWidgetItem(ui->selectTreeWidget);
        auto select_set_str = QString::fromStdString(select.first.first) + " -> ";
        root_item->setText(0, select_set_str);

        QVector<QTreeWidgetItem*> child_items;
        auto set_it = select.second.cbegin();
        for (size_t i = 0; i < select.second.size(); ++i) {
            auto* child_item = new QTreeWidgetItem(root_item);
            if (i < select.first.second.size()) {
                child_item->setText(1, QString::fromStdString(select.first.second[i]));
            }

            if (i == select.second.size() - 1) {
                child_item->setText(2, QString::fromStdString(*set_it));
            } else {
                child_item->setText(2, QString::fromStdString(*set_it));
                set_it++;
            }
            child_items.emplace_back(child_item);
        }
    }
}