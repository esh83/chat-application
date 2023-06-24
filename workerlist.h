#ifndef WORKERLIST_H
#define WORKERLIST_H

#include <QObject>
#include "config.h"
#include "queries.h"
#include "requesthandler.h"

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

signals:
    void listUserReady(QVector<QString> result);
    void listGroupReady(QVector<QString> result);
    void listChannelReady(QVector<QString> result);
    void failed();

};

#endif // WORKERLIST_H