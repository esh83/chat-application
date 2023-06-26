#include "workerlist.h"
#include <QCoreApplication>

WorkerList::WorkerList(QString token,QObject *parent)
    : QObject{parent} , m_token{token}
{

}

void WorkerList::getList(int chatType , QString endpoint)
{
    //GET LIST FROM API
    RequestHandler *req_handler = new RequestHandler(this);
    connect(req_handler,&RequestHandler::errorOccured,[=](QString err){
        qDebug()<< "error fetching list data from api : " <<  err;
        emit failed();
    });
    connect(req_handler,&RequestHandler::dataReady,[=](QJsonObject jsonObj){
        QString code = jsonObj.value("code").toString();
        if (code == "200")
        {

            DB::deleteTblChatsList(chatType);
            for (auto it = jsonObj.begin(); it != jsonObj.end(); it++) {
                if (it.value().isObject()) {
                    QString name;
                    switch (chatType) {
                    case PERSONAL_CHAT:
                        name = it.value().toObject().value("src").toString();
                        break;
                    case GROUP_CHAT:
                        name = it.value().toObject().value("group_name").toString();
                        break;
                    case CHANNEL_CHAT:
                        name = it.value().toObject().value("channel_name").toString();
                        break;
                    }


                    //INSERT DATA TO LOCAL DATABASE
                        try {
                            DB::insertTblChatsList(name, "Jogn Doe", chatType);
                        }
                        catch (QString& err) {
                            qDebug() << err;
                        }
                }
            }
            emit success();
        }
        else {
            qDebug() << "error fetching list data from api :" << code;
        }

    });
    connect(req_handler,&RequestHandler::done,[=](){
        //READ LIST FROM DATABASE
        QVector<QString> result;
        try {
            QVector<DB::TableChatsList> list = DB::selectTblChatsList(chatType);
            for (auto it = list.begin(); it != list.end(); it++)
                result.push_back((*it).username);
        }
        catch (QString& err) {
            qDebug() << err;
        }
        switch (chatType) {
        case CHANNEL_CHAT:
            emit listChannelReady(result);
            break;
        case GROUP_CHAT:
            emit listGroupReady(result);
            break;
        case PERSONAL_CHAT:
            emit listUserReady(result);
            break;
        default:
            break;
        }
    });
    req_handler->fetchData(QString(API_ADRESS) + endpoint + "?token=" + m_token);

}

void WorkerList::getUserList()
{
     getList(PERSONAL_CHAT, "/getuserlist");
}
void WorkerList::getChannelList()
{
     getList(CHANNEL_CHAT, "/getchannellist");
}

void WorkerList::openDB()
{
     QSqlDatabase::removeDatabase("worker_db");
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
void WorkerList::getGroupList()
{
     getList(GROUP_CHAT, "/getgrouplist");
}
