#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H
#include <QString>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>
#include "config.h"

class RequestHandler: public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager* m_manager;
public:
    void fetchData(const QString &url);
    RequestHandler(QObject *parent = nullptr);
    ~RequestHandler();

public slots:
    void handleData(QNetworkReply* reply);
signals:
    void dataReady(QJsonObject);
    void errorOccured(QString err);
    void done();
};

#endif // REQUESTHANDLER_H
