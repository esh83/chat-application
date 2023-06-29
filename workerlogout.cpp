#include "workerlogout.h"

WorkerLogout::WorkerLogout(QString username, QString password,QObject *parent)
    : QObject{parent},m_username{username},m_password{password}
{

}

void WorkerLogout::logout()
{
    RequestHandler *req_handler = new RequestHandler(this);
    connect(req_handler,&RequestHandler::errorOccured,[=](QString err){
        qDebug()<<err;
        emit failed(err);
    });
    connect(req_handler,&RequestHandler::dataReady,[=](QJsonObject jsonObj ){
        QString message = jsonObj.value("message").toString();
        QString code = jsonObj.value("code").toString();
        if (code == "200")
        {
            //RESET LOCAL DATABASE
            DB::emptyTblInfo();
            DB::emptyTblChatsList();
            DB::emptyTblChats();
            emit done();
            emit success();

        }
        else{
            emit done();
            emit failed(message);
        }

    });
    req_handler->fetchData(QString(API_ADRESS)+"/logout?username="+m_username+"&password="+m_password);
}
