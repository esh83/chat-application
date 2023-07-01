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
    explicit WorkerList(QString token ="" ,QObject *parent = nullptr);
    QString m_token;
private:
    void getList(int chatType,QString endpoint);
    bool showMessage;
public slots:
    void getUserList();
    void getGroupList();
    void getChannelList();
    void openDB();

signals:
    void listUserReady(QVector<QString> result);
    void listGroupReady(QVector<QString> result);
    void listChannelReady(QVector<QString> result);
    void failed();
    void success();
    void expired ();


};

#endif // WORKERLIST_H
