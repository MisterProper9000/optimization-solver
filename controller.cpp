#include "controller.h"
#include "sqlconnectiondialog.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QtSql>
#include <QMessageBox>
#include <float.h>
#include <limits.h>

Controller::Controller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controller)
{
    ui->setupUi(this);
}

Controller::~Controller()
{
    delete ui;
}

QSqlError Controller::changeConnection(const QString &driver, const QString &dbName, const QString &host,
                             const QString &user, const QString &passwd, int port)
{
    static int cCount = 0;

    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver, QString("optimization_solver%1").arg(++cCount));
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    if (!db.open(user, passwd)) {
        err = db.lastError();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(QString("optimization_solver%1").arg(cCount));
    } else if (select()){
        //insButton->setEnabled(true);
    } else {
        //insButton->setEnabled(false);
        db.close();
        QSqlDatabase::removeDatabase(QString("optimization_solver%1").arg(cCount));
    }
    return err;
}

void Controller::changeConnection(){
    SqlConnectionDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return;
    QSqlError err = changeConnection(dialog.driverName(), dialog.databaseName(), dialog.hostName(),
                                     dialog.userName(), dialog.password(), dialog.port());
    if (err.type() != QSqlError::NoError)
        QMessageBox::warning(this, tr("Unable to open database"), tr("An error occurred while "
                             "opening the connection: ") + err.text());

}

bool Controller::select()
{
    QStringList connectionNames = QSqlDatabase::connectionNames();// список всех подключений, возьмем последнее
    QSqlDatabase currentDB = QSqlDatabase::database(connectionNames.value(connectionNames.count() - 1));
    if (currentDB.tables().count() == 0) {
       QSqlQueryModel *m = new QSqlQueryModel(ui->table);
       m->setQuery(QSqlQuery("create table solvers(Name text, Description text, Path text);", currentDB));
       if (m->lastError().type() != QSqlError::NoError) {
           emit statusMessage(m->lastError().text());
           delete m;
           return false;
       }
       delete m;
    }
    QStringList tables = currentDB.tables();
    QSqlTableModel *model = new QSqlTableModel(ui->table, currentDB);
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->setTable(currentDB.driver()->escapeIdentifier(tables.at(0), QSqlDriver::TableName));
    model->select();
    if (model->lastError().type() != QSqlError::NoError) {
        emit statusMessage(model->lastError().text());
        delete model;
        return false;
    }
    if (!checkTableModel(model->record())) {
        emit statusMessage(tr("Bad table."));
        delete model;
        return false;
    }
    ui->table->setModel(model);
    ui->table->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);

    connect(ui->table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            this, SLOT(currentChanged()));

    if (tables.count() > 1)
        emit statusMessage(tr("Query OK. Count of tables is more than one. Using first table."));
    else
        emit statusMessage(tr("Query OK."));

    updateActions();
    return true;
}

void Controller::updateActions()
{
    bool enableIns = qobject_cast<QSqlTableModel *>(ui->table->model());
    bool enableDel = enableIns && ui->table->currentIndex().isValid();

    ui->action_Insert_Row->setEnabled(true);
    ui->action_Delete_Row->setEnabled(true);
}

bool Controller::checkTableModel(QSqlRecord record) {
    QString fieldNames[3] = {"Name", "Description", "File"};
    if (record.count() != 3)
        return false;
    for (int i = 0; i < 3; i++){
        if (record.fieldName(i).compare(fieldNames[i]) != 0){
            return false;
        }
    }
    return true;
}

void Controller::insertRow()
{
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(ui->table->model());
    if (!model)
        return;

    QModelIndex insertIndex = ui->table->currentIndex();
    int row = insertIndex.row() == -1 ? 0 : insertIndex.row();
    model->insertRow(row);
    insertIndex = model->index(row, 0);
    ui->table->setCurrentIndex(insertIndex);
    ui->table->edit(insertIndex);
}

void Controller::deleteRow()
{
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(ui->table->model());
    if (!model)
        return;

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    QModelIndexList currentSelection = ui->table->selectionModel()->selectedIndexes();
    for (int i = 0; i < currentSelection.count(); ++i) {
        if (currentSelection.at(i).column() != 0)
            continue;
        model->removeRow(currentSelection.at(i).row());
    }

    model->submitAll();
    model->setEditStrategy(QSqlTableModel::OnRowChange);

    updateActions();
}

void Controller::on_SolveButton_clicked()
{

}

void Controller::on_DrawButton_clicked()
{

}

void Controller::on_action_Insert_Row_triggered()
{
    insertRow();
}

void Controller::on_action_Delete_Row_triggered()
{
    deleteRow();
}

void Controller::on_pushButton_clicked()
{
    insertRow();
}

void Controller::on_pushButton_2_clicked()
{
    deleteRow();
}

void Controller::on_browseButton_clicked() //problem path
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select 'problem' file", "", "DLL (*.dll)");
    ui->problemEdit->setText(fileName);
}
