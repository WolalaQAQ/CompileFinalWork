//
// Created by Adminstrator on 24-6-13.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ir_generator_gui.h" resolved

#include "gui/ir_generator_gui.h"
#include "gui/mainwindow.h"
#include "ui_ir_generator_gui.h"

#include "gui/dialog.h"

#include <QTableWidget>

IRGeneratorGUI::IRGeneratorGUI(MainWindow *main_window, QWidget *parent) : main_window_(main_window),
                                                                           QWidget(parent),
                                                                           ui(new Ui::IRGeneratorGUI) {
    ui->setupUi(this);

    connect(main_window_, &MainWindow::sendCode, this, [this](const QString &code) {
        code_qstring_ = code;
    });
    connect(ui->startPushButton, &QPushButton::clicked, this, &IRGeneratorGUI::generateIR);
}

IRGeneratorGUI::~IRGeneratorGUI() {
    delete ui;
}

void IRGeneratorGUI::generateIR() {
    scanner_.scan(code_qstring_.toStdString());
    auto tokens = scanner_.getTokens();

    ir_generator_.form(tokens);

    dialog::showDialog("IR generated successfully.", QMessageBox::Information);

    auto quadruples = ir_generator_.getQuadruples();
    auto symbol_table = ir_generator_.getSymbolTable();

    ui->quadrupleTableWidget->setRowCount(quadruples.size());
    for (const auto& quadruple : quadruples) {
        auto* op_item = new QTableWidgetItem(QString::fromStdString(quadruple.op));
        ui->quadrupleTableWidget->setItem(quadruple.index - 1, 0, op_item);

        auto x1_str = quadruple.x1;
        if (quadruple.x1_type != "_") {
            x1_str += " ( " + quadruple.x1_type + " )";
        }
        auto* x1_item = new QTableWidgetItem(QString::fromStdString(x1_str));
        ui->quadrupleTableWidget->setItem(quadruple.index - 1, 1, x1_item);

        auto x2_str = quadruple.x2;
        if (quadruple.x2_type != "_") {
            x2_str += " ( " + quadruple.x2_type + " )";
        }
        auto* x2_item = new QTableWidgetItem(QString::fromStdString(x2_str));
        ui->quadrupleTableWidget->setItem(quadruple.index - 1, 2, x2_item);

        auto result_str = quadruple.result;
        if (quadruple.result_type != "_") {
            result_str += " ( " + quadruple.result_type + " )";
        }
        auto* result_item = new QTableWidgetItem(QString::fromStdString(result_str));
        ui->quadrupleTableWidget->setItem(quadruple.index - 1, 3, result_item);
    }

    ui->symbolTableWidget->setRowCount(symbol_table.size());
    for (const auto& symbol : symbol_table) {
        auto* name_item = new QTableWidgetItem(QString::fromStdString(symbol.name));
        ui->symbolTableWidget->setItem(symbol.index - 1, 0, name_item);

        auto* type_item = new QTableWidgetItem(QString::fromStdString(symbol.type));
        ui->symbolTableWidget->setItem(symbol.index - 1, 1, type_item);

        auto* category_item = new QTableWidgetItem(QString::fromStdString(symbol.category));
        ui->symbolTableWidget->setItem(symbol.index - 1, 2, category_item);

        auto* address_item = new QTableWidgetItem(QString::number(symbol.address));
        ui->symbolTableWidget->setItem(symbol.index - 1, 3, address_item);
    }

    auto vall = ir_generator_.getVall();
    ui->vallTableWidget->setRowCount(vall.size());
    int row = 0;
    while (!vall.empty()) {
        auto [name, value] = vall.top();
        vall.pop();

        auto* name_item = new QTableWidgetItem(QString::fromStdString(name));
        ui->vallTableWidget->setItem(row, 0, name_item);

        auto* value_item = new QTableWidgetItem(QString::number(value));
        ui->vallTableWidget->setItem(row, 1, value_item);

        ++row;
    }

}
