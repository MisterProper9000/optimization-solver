#ifndef SQLCONNECTIONDIALOG_H
#define SQLCONNECTIONDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QMessageBox>

#include "ui_sqlconnectiondialog.h"

namespace Ui {
    class SqlConnectionDialog;
}

class SqlConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SqlConnectionDialog(QWidget *parent = 0);
    ~SqlConnectionDialog();
    QString driverName() const;
    QString databaseName() const;
    QString userName() const;
    QString password() const;
    QString hostName() const;
    int port() const;
    bool useInMemoryDatabase() const;

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked() { reject(); }

private:
    Ui::SqlConnectionDialog *ui;
};

#endif // SQLCONNECTIONDIALOG_H
