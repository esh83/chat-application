#ifndef WORKERLIST_H
#define WORKERLIST_H

#include <QObject>
#include "config.h"
#include "queries.h"
#include "requesthandler.h"
#include <QCoreApplication>
#define PERSONAL_TAB 0
#define CHANNEL_TAB 1
#define GROUP_TAB 2

class WorkerList : public QObject
{
    Q_OBJECT
public :
    explicit WorkerList(QString token ="" ,QObject *parent = nullptr);
    QString m_token;
     int m_current_tab=PERSONAL_TAB;
private:
    void getList(int chatType,QString endpoint);
    bool showMessage;

public slots:
    void getUserList();
    void getGroupList();
    void getChannelList();
    void getCurrentList();
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
