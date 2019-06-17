/********************************************************************************
** Form generated from reading UI file 'sqlconnectiondialog.ui'
**
** Created: Sun 16. Jun 18:05:41 2019
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SQLCONNECTIONDIALOG_H
#define UI_SQLCONNECTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SqlConnectionDialog
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *connGroupBox;
    QGridLayout *gridLayout;
    QLabel *textLabel4;
    QLineEdit *editDatabase;
    QSpinBox *portSpinBox;
    QLabel *textLabel3;
    QLineEdit *editPassword;
    QLineEdit *editUsername;
    QLineEdit *editHostname;
    QLabel *textLabel5;
    QLabel *textLabel5_2;
    QLabel *textLabel4_2;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QCheckBox *dbCheckBox;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem1;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SqlConnectionDialog)
    {
        if (SqlConnectionDialog->objectName().isEmpty())
            SqlConnectionDialog->setObjectName(QString::fromUtf8("SqlConnectionDialog"));
        SqlConnectionDialog->resize(325, 327);
        vboxLayout = new QVBoxLayout(SqlConnectionDialog);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
        vboxLayout->setContentsMargins(8, 8, 8, 8);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        connGroupBox = new QGroupBox(SqlConnectionDialog);
        connGroupBox->setObjectName(QString::fromUtf8("connGroupBox"));
        gridLayout = new QGridLayout(connGroupBox);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
        gridLayout->setContentsMargins(8, 8, 8, 8);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textLabel4 = new QLabel(connGroupBox);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));

        gridLayout->addWidget(textLabel4, 1, 0, 1, 1);

        editDatabase = new QLineEdit(connGroupBox);
        editDatabase->setObjectName(QString::fromUtf8("editDatabase"));

        gridLayout->addWidget(editDatabase, 0, 1, 1, 1);

        portSpinBox = new QSpinBox(connGroupBox);
        portSpinBox->setObjectName(QString::fromUtf8("portSpinBox"));
        portSpinBox->setMinimum(-1);
        portSpinBox->setMaximum(65535);
        portSpinBox->setValue(-1);

        gridLayout->addWidget(portSpinBox, 4, 1, 1, 1);

        textLabel3 = new QLabel(connGroupBox);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));

        gridLayout->addWidget(textLabel3, 0, 0, 1, 1);

        editPassword = new QLineEdit(connGroupBox);
        editPassword->setObjectName(QString::fromUtf8("editPassword"));
        editPassword->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(editPassword, 2, 1, 1, 1);

        editUsername = new QLineEdit(connGroupBox);
        editUsername->setObjectName(QString::fromUtf8("editUsername"));

        gridLayout->addWidget(editUsername, 1, 1, 1, 1);

        editHostname = new QLineEdit(connGroupBox);
        editHostname->setObjectName(QString::fromUtf8("editHostname"));

        gridLayout->addWidget(editHostname, 3, 1, 1, 1);

        textLabel5 = new QLabel(connGroupBox);
        textLabel5->setObjectName(QString::fromUtf8("textLabel5"));

        gridLayout->addWidget(textLabel5, 3, 0, 1, 1);

        textLabel5_2 = new QLabel(connGroupBox);
        textLabel5_2->setObjectName(QString::fromUtf8("textLabel5_2"));

        gridLayout->addWidget(textLabel5_2, 4, 0, 1, 1);

        textLabel4_2 = new QLabel(connGroupBox);
        textLabel4_2->setObjectName(QString::fromUtf8("textLabel4_2"));

        gridLayout->addWidget(textLabel4_2, 2, 0, 1, 1);


        vboxLayout->addWidget(connGroupBox);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        dbCheckBox = new QCheckBox(SqlConnectionDialog);
        dbCheckBox->setObjectName(QString::fromUtf8("dbCheckBox"));

        hboxLayout->addWidget(dbCheckBox);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacerItem1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);

        okButton = new QPushButton(SqlConnectionDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setDefault(true);

        hboxLayout1->addWidget(okButton);

        cancelButton = new QPushButton(SqlConnectionDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout1->addWidget(cancelButton);


        vboxLayout->addLayout(hboxLayout1);

#ifndef QT_NO_SHORTCUT
        textLabel4->setBuddy(editUsername);
        textLabel3->setBuddy(editDatabase);
        textLabel5->setBuddy(editHostname);
        textLabel5_2->setBuddy(portSpinBox);
        textLabel4_2->setBuddy(editPassword);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(editDatabase, editUsername);
        QWidget::setTabOrder(editUsername, editPassword);
        QWidget::setTabOrder(editPassword, editHostname);
        QWidget::setTabOrder(editHostname, portSpinBox);
        QWidget::setTabOrder(portSpinBox, dbCheckBox);
        QWidget::setTabOrder(dbCheckBox, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(SqlConnectionDialog);

        QMetaObject::connectSlotsByName(SqlConnectionDialog);
    } // setupUi

    void retranslateUi(QDialog *SqlConnectionDialog)
    {
        SqlConnectionDialog->setWindowTitle(QApplication::translate("SqlConnectionDialog", "Connect...", 0, QApplication::UnicodeUTF8));
        connGroupBox->setTitle(QApplication::translate("SqlConnectionDialog", "Connection settings", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("SqlConnectionDialog", "&Username:", 0, QApplication::UnicodeUTF8));
        portSpinBox->setSpecialValueText(QApplication::translate("SqlConnectionDialog", "Default", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("SqlConnectionDialog", "Database Name:", 0, QApplication::UnicodeUTF8));
        textLabel5->setText(QApplication::translate("SqlConnectionDialog", "&Hostname:", 0, QApplication::UnicodeUTF8));
        textLabel5_2->setText(QApplication::translate("SqlConnectionDialog", "P&ort:", 0, QApplication::UnicodeUTF8));
        textLabel4_2->setText(QApplication::translate("SqlConnectionDialog", "&Password:", 0, QApplication::UnicodeUTF8));
        dbCheckBox->setText(QApplication::translate("SqlConnectionDialog", "Us&e predefined in-memory database", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("SqlConnectionDialog", "&OK", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("SqlConnectionDialog", "&Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SqlConnectionDialog: public Ui_SqlConnectionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SQLCONNECTIONDIALOG_H
