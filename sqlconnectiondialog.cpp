#include "sqlconnectiondialog.h"

SqlConnectionDialog::SqlConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SqlConnectionDialog)
{
    ui->setupUi(this);
}

SqlConnectionDialog::~SqlConnectionDialog()
{
    delete ui;
}

QString SqlConnectionDialog::driverName() const
{
    return QString::fromLatin1("QMYSQL");
}

QString SqlConnectionDialog::databaseName() const
{
    return ui->editDatabase->text();
}

QString SqlConnectionDialog::userName() const
{
    return ui->editUsername->text();
}

QString SqlConnectionDialog::password() const
{
    return ui->editPassword->text();
}

QString SqlConnectionDialog::hostName() const
{
    return ui->editHostname->text();
}

int SqlConnectionDialog::port() const
{
    return ui->portSpinBox->value();
}

bool SqlConnectionDialog::useInMemoryDatabase() const
{
    return ui->dbCheckBox->isChecked();
}

void SqlConnectionDialog::on_okButton_clicked()
{
   accept();
}
