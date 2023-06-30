#ifndef WORKERLOGOUT_H
#define WORKERLOGOUT_H
#include "config.h"
#include "queries.h"
#include "requesthandler.h"
#include <QObject>

class WorkerLogout : public QObject
{
    Q_OBJECT
public :
    explicit WorkerLogout(QString username ,QString password,QObject *parent = nullptr);
    QString m_token;
private:
   QString m_username;
    QString m_password;
public slots:
    void logout();

signals:
    void failed(QString err);
    void done();
    void success();

};

#endif // WORKERLOGOUT_H
