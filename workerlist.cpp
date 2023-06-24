#include "workerlist.h"

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
            QVector<QString> result;

            //DB::deleteTblChatsList(m_chatType);
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
                    result.push_back(name);

                    //INSERT DATA TO LOCAL DATABASE
                    /*try {
                            DB::insertTblChatsList(name, "Jogn Doe", m_chatType);
                        }
                        catch (QString& err) {
                            qDebug() << err;
                        }*/
                }
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

        }
        else {
            qDebug() << "error fetching list data from api :" << code;
        }

    });
    req_handler->fetchData(QString(API_ADRESS) + endpoint + "?token=" + m_token);
    //READ LIST FROM LOCAL DATABASE

    /* try {
            QVector<DB::TableChatsList> list = DB::selectTblChatsList(m_chatType);
            for (auto it = list.begin(); it != list.end(); it++)
                vec.push_back((*it).username);
        }
        catch (QString& err) {
            qDebug() << err;
        }*/

}

void WorkerList::getUserList()
{
     getList(PERSONAL_CHAT, "/getuserlist");
}
void WorkerList::getChannelList()
{
     getList(CHANNEL_CHAT, "/getchannellist");
}
void WorkerList::getGroupList()
{
     getList(GROUP_CHAT, "/getgrouplist");
}
