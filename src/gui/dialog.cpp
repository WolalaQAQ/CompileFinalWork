//
// Created by wolala on 24-5-19.
//
#include "gui/dialog.h"

namespace dialog {
    void showDialog(const QString &message, QMessageBox::Icon icon) {
        QMessageBox msgBox;
        msgBox.setIcon(icon);

        switch (icon) {
            case QMessageBox::Critical:
                msgBox.setText("Error");
                break;

            case QMessageBox::Information:
                msgBox.setText("Info");
                break;

            case QMessageBox::Warning:
                msgBox.setText("Warning");
                break;

            case QMessageBox::Question:
                msgBox.setText("Question");
                break;
            default:
                break;
        }

        msgBox.setInformativeText(message);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

}// namespace dialog