#ifndef WORKERCHAT_H
#define WORKERCHAT_H

#include <QObject>
#include "config.h"
#include "queries.h"
#include "requesthandler.h"
#include <QCoreApplication>

struct chatMsg {
    bool isRight;
    QString msg;
};

class WorkerChat : public QObject
{
    Q_OBJECT
public :
    explicit WorkerChat(QString token ="" ,QObject *parent = nullptr);
    QString m_token;
    QString m_endpoint;
    QString m_des;
    QString m_username;
    QString m_body;
    int m_type;
public slots:
    void getChats();
    void sendChat();
    void openDB();

signals:
    void chatsReady(QVector<chatMsg> result);
    void failedRead();
    void failedWrite(QString err);
    void chatSended();


};

#endif // WORKERCHAT_H
