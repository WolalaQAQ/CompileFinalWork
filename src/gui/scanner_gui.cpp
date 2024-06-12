//
// Created by wolala on 24-6-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_scanner_gui.h" resolved

#include "gui/scanner_gui.h"
#include "ui_scanner_gui.h"

#include <QDebug>
#include <QTableWidget>

#include <map>

ScannerGUI::ScannerGUI(MainWindow *main_window, QWidget *parent) :
	main_window_(main_window), QWidget(parent), ui(new Ui::ScannerGUI) {
	ui->setupUi(this);

	connect(main_window_, &MainWindow::sendCode, this, [this] (const QString &code) {
	  code_qstring_ = code;
	});
	connect(ui->startPushButton, &QPushButton::clicked, this, &ScannerGUI::scanCode);
}

ScannerGUI::~ScannerGUI() {
	delete ui;
}

void ScannerGUI::scanCode() {
	std::cout << "Code to scan: \n" << code_qstring_.toStdString() << std::endl;
	auto keywordList = scanner_.getKeywordList();
	auto delimiterList = scanner_.getDelimiterList();

	// 设置表格的行数
	ui->kTableWidget->setRowCount(keywordList.size());
	ui->pTableWidget->setRowCount(delimiterList.size());

	// 先填充关键字和界符表
	for (int i = 0; i < keywordList.size(); ++i) {
		auto* keyword_item = new QTableWidgetItem(QString::fromStdString(keywordList[i]));
		ui->kTableWidget->setItem(i, 0, keyword_item);
	}

	for (int i = 0; i < delimiterList.size(); ++i) {
		auto* delimiter_item = new QTableWidgetItem(QString::fromStdString(delimiterList[i]));
		ui->pTableWidget->setItem(i, 0, delimiter_item);
	}

	auto answer_tokens = scanner_.get_AnswerTokens(code_qstring_.toStdString());
	auto tokens = scanner_.getTokens();
	// 新建一个mao来记录tokens，防止重复
	std::map<std::string, TokenInfo> token_map;

	// 填充词法分析结果
	for (const auto &token : tokens) {
		// 如果该token已经存在，则跳过
		if (token_map.find(token.first) != token_map.end()) {
			continue;
		}

		auto token_type = token.second.type;
		if (token_type != "I" && token_type != "C1" && token_type != "C2") {
			continue;
		}
		auto* token_item = new QTableWidgetItem(QString::fromStdString(token.first));

		if (token_type == "I") {
			ui->iTableWidget->setRowCount(ui->iTableWidget->rowCount() + 1);
			ui->iTableWidget->setItem(ui->iTableWidget->rowCount() - 1, 0, token_item);
		} else if (token_type == "C1") {
			auto* int_type_item = new QTableWidgetItem(QString("整数"));
			ui->cTableWidget->setRowCount(ui->cTableWidget->rowCount() + 1);
			ui->cTableWidget->setItem(ui->cTableWidget->rowCount() - 1, 0, int_type_item);
			ui->cTableWidget->setItem(ui->cTableWidget->rowCount() - 1, 1, token_item);
		} else if (token_type == "C2") {
			auto* real_type_item = new QTableWidgetItem(QString("实数"));
			ui->cTableWidget->setRowCount(ui->cTableWidget->rowCount() + 1);
			ui->cTableWidget->setItem(ui->cTableWidget->rowCount() - 1, 0, real_type_item);
			ui->cTableWidget->setItem(ui->cTableWidget->rowCount() - 1, 1, token_item);
		}

		token_map[token.first] = token.second;
	}

}