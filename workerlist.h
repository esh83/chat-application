#ifndef WORKERLIST_H
#define WORKERLIST_H

#include <QObject>
#include "config.h"
#include "queries.h"
#include "requesthandler.h"
#include <QCoreApplication>

class WorkerList : public QObject
{
    Q_OBJECT
public :
    WorkerList(QString token ="" ,QObject *parent = nullptr);
    QString m_token;
private:
    void getList(int chatType,QString endpoint);
public slots:
    void getUserList();
    void getGroupList();
    void getChannelList();
    void openDB(){
        QString path = QCoreApplication::applicationDirPath();
        QString dbPath = path + "/data.db";
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",  "worker_db");
        db.setDatabaseName(dbPath);
        if (!db.open()) {
            qDebug() << db.lastError().text();
        } else {
            qDebug() << "Database opened successfully";
        }
    }

signals:
    void listUserReady(QVector<QString> result);
    void listGroupReady(QVector<QString> result);
    void listChannelReady(QVector<QString> result);
    void failed();

};

#endif // WORKERLIST_H
