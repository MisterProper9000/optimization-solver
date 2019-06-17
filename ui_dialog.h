/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created: Fri 14. Jun 22:41:01 2019
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *parametersText;
    QComboBox *methodsBox;
    QPushButton *solveButton;
    QPushButton *solveButton_2;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(704, 490);
        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(330, 450, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        parametersText = new QLineEdit(Dialog);
        parametersText->setObjectName(QString::fromUtf8("parametersText"));
        parametersText->setGeometry(QRect(40, 140, 341, 27));
        methodsBox = new QComboBox(Dialog);
        methodsBox->setObjectName(QString::fromUtf8("methodsBox"));
        methodsBox->setGeometry(QRect(40, 60, 341, 27));
        solveButton = new QPushButton(Dialog);
        solveButton->setObjectName(QString::fromUtf8("solveButton"));
        solveButton->setGeometry(QRect(40, 240, 112, 34));
        solveButton->setDefault(false);
        solveButton->setFlat(false);
        solveButton_2 = new QPushButton(Dialog);
        solveButton_2->setObjectName(QString::fromUtf8("solveButton_2"));
        solveButton_2->setGeometry(QRect(260, 240, 112, 34));
        solveButton_2->setDefault(false);
        solveButton_2->setFlat(false);

        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        solveButton->setText(QApplication::translate("Dialog", "Solve", 0, QApplication::UnicodeUTF8));
        solveButton_2->setText(QApplication::translate("Dialog", "Draw", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
