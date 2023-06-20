#include "requesthandler.h"



RequestHandler::RequestHandler(QObject *parent) :QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager,&QNetworkAccessManager::finished,this ,&RequestHandler::handleData);

}
void RequestHandler::fetchData(const QString &url)
{
    QNetworkRequest request(url);
    m_manager->get(request);

}

RequestHandler::~RequestHandler()
{

}

void RequestHandler::handleData(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        emit errorOccured(reply->errorString());
    }
    else
    {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();
        emit dataReady(jsonObj);
    }
  reply->deleteLater();
}
