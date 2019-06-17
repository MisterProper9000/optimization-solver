#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "controller.h"

void addConnectionsFromCommandline(const QStringList &args, Controller *controller)
{
    for (int i = 1; i < args.count(); ++i) {
        QUrl url(args.at(i), QUrl::TolerantMode);
        if (!url.isValid()) {
            qWarning("Invalid URL: %s", qPrintable(args.at(i)));
            continue;
        }
        QSqlError err = controller->changeConnection(url.scheme(), url.path().mid(1), url.host(),
                                               url.userName(), url.password(), url.port(-1));
        if (err.type() != QSqlError::NoError)
            qDebug() << "Unable to open connection:" << err;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow w;
    w.setWindowTitle(QObject::tr("Optimization Solver"));

    Controller c(&w);
    w.setCentralWidget(&c);

    QMenu *fileMenu = w.menuBar()->addMenu(QObject::tr("&File"));
    fileMenu->addAction(QObject::tr("Change &Connection..."), &c, SLOT(changeConnection()));
    fileMenu->addSeparator();
    fileMenu->addAction(QObject::tr("&Quit"), &a, SLOT(quit()));

    QObject::connect(&c, SIGNAL(statusMessage(QString)),
                     w.statusBar(), SLOT(showMessage(QString)));

    addConnectionsFromCommandline(a.arguments(), &c);
    w.show();
    if (QSqlDatabase::connectionNames().isEmpty())
        QMetaObject::invokeMethod(&c, "changeConnection", Qt::QueuedConnection);
    return a.exec();
}
