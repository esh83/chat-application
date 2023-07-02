#ifndef WORKEROTHER_H
#define WORKEROTHER_H
#include "config.h"
#include "queries.h"
#include "requesthandler.h"
#include <QObject>

class WorkerOther : public QObject
{
    Q_OBJECT
public :
    explicit WorkerOther(QString username ,QString password,QObject *parent = nullptr);
    QString m_token;
private:
   QString m_username;
    QString m_password;
public slots:
    void logout();
    void openDB();

signals:
    void failed(QString err);
    void done();
    void success();

};

#endif // WORKEROTHER_H
