//
// Created by wolala on 24-5-19.
//
#include "gui/dialogs.h"

namespace dialogs {
    void showErrorDialog(const QString &message) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);// 设置图标类型为错误
        msgBox.setText("Error");
        msgBox.setInformativeText(message);
        msgBox.setStandardButtons(QMessageBox::Ok);// 设置标准按钮
        msgBox.setDefaultButton(QMessageBox::Ok);  // 设置默认按钮
        msgBox.exec();                             // 显示消息框
    }

    void showInfoDialog(const QString &message) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);// 设置图标类型为信息
        msgBox.setText("Info");
        msgBox.setInformativeText(message);
        msgBox.setStandardButtons(QMessageBox::Ok);// 设置标准按钮
        msgBox.setDefaultButton(QMessageBox::Ok);  // 设置默认按钮
        msgBox.exec();                             // 显示消息框
    }
}// namespace dialogs