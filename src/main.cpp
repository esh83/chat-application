#include "chatpage.h"
#include "setupmain.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QMutex>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString path = QCoreApplication::applicationDirPath();
    QString dbPath = path + "/data.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    if (!db.open()) {
        qDebug() << db.lastError().text();
        return 1;
    } else {
        qDebug() << "Database opened successfully";
    }
    SetupMain w;
    w.show();
    return a.exec();
}
