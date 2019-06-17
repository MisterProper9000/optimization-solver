/********************************************************************************
** Form generated from reading UI file 'controller.ui'
**
** Created: Mon 17. Jun 17:51:36 2019
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Controller
{
public:
    QAction *action_Insert_Row;
    QAction *action_Delete_Row;
    QPushButton *SolveButton;
    QPushButton *DrawButton;
    QComboBox *taskBox;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *problemEdit;
    QLabel *label_3;
    QPushButton *browseButton;
    QTableView *table;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *Controller)
    {
        if (Controller->objectName().isEmpty())
            Controller->setObjectName(QString::fromUtf8("Controller"));
        Controller->resize(807, 511);
        Controller->setMinimumSize(QSize(690, 508));
        Controller->setFocusPolicy(Qt::NoFocus);
        action_Insert_Row = new QAction(Controller);
        action_Insert_Row->setObjectName(QString::fromUtf8("action_Insert_Row"));
        action_Delete_Row = new QAction(Controller);
        action_Delete_Row->setObjectName(QString::fromUtf8("action_Delete_Row"));
        SolveButton = new QPushButton(Controller);
        SolveButton->setObjectName(QString::fromUtf8("SolveButton"));
        SolveButton->setGeometry(QRect(40, 460, 112, 34));
        DrawButton = new QPushButton(Controller);
        DrawButton->setObjectName(QString::fromUtf8("DrawButton"));
        DrawButton->setGeometry(QRect(190, 460, 112, 34));
        taskBox = new QComboBox(Controller);
        taskBox->setObjectName(QString::fromUtf8("taskBox"));
        taskBox->setGeometry(QRect(40, 410, 98, 27));
        label = new QLabel(Controller);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 20, 70, 21));
        label_2 = new QLabel(Controller);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 310, 70, 21));
        problemEdit = new QLineEdit(Controller);
        problemEdit->setObjectName(QString::fromUtf8("problemEdit"));
        problemEdit->setGeometry(QRect(40, 340, 461, 27));
        label_3 = new QLabel(Controller);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(36, 380, 70, 21));
        browseButton = new QPushButton(Controller);
        browseButton->setObjectName(QString::fromUtf8("browseButton"));
        browseButton->setGeometry(QRect(540, 337, 112, 34));
        table = new QTableView(Controller);
        table->setObjectName(QString::fromUtf8("table"));
        table->setEnabled(true);
        table->setGeometry(QRect(40, 60, 461, 241));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(table->sizePolicy().hasHeightForWidth());
        table->setSizePolicy(sizePolicy);
        table->setFocusPolicy(Qt::WheelFocus);
        table->setContextMenuPolicy(Qt::ActionsContextMenu);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        pushButton = new QPushButton(Controller);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(530, 60, 112, 34));
        pushButton_2 = new QPushButton(Controller);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(530, 120, 112, 34));

        retranslateUi(Controller);

        taskBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Controller);
    } // setupUi

    void retranslateUi(QWidget *Controller)
    {
        Controller->setWindowTitle(QApplication::translate("Controller", "Optimization solver", 0, QApplication::UnicodeUTF8));
        action_Insert_Row->setText(QApplication::translate("Controller", "&Insert Row", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_Insert_Row->setToolTip(QApplication::translate("Controller", "Insert Row", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        action_Insert_Row->setStatusTip(QApplication::translate("Controller", "Insert new row", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_Insert_Row->setShortcut(QApplication::translate("Controller", "Ctrl+;", 0, QApplication::UnicodeUTF8));
        action_Delete_Row->setText(QApplication::translate("Controller", "&Delete Row", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_Delete_Row->setToolTip(QApplication::translate("Controller", "Delete existing row", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        SolveButton->setText(QApplication::translate("Controller", "Solve", 0, QApplication::UnicodeUTF8));
        DrawButton->setText(QApplication::translate("Controller", "Draw", 0, QApplication::UnicodeUTF8));
        taskBox->clear();
        taskBox->insertItems(0, QStringList()
         << QApplication::translate("Controller", "Min", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Controller", "Max", 0, QApplication::UnicodeUTF8)
        );
        label->setText(QApplication::translate("Controller", "Solvers", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Controller", "Problem", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Controller", " Min/max", 0, QApplication::UnicodeUTF8));
        browseButton->setText(QApplication::translate("Controller", "Browse...", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Controller", "Insert row", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Controller", "Delete row", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Controller: public Ui_Controller {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLLER_H
