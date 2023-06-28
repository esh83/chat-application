#include "workerchat.h"


WorkerChat::WorkerChat(QString token, QObject *parent) : QObject{parent} , m_token{token}
{

}

void WorkerChat::getChats()
{
    if(m_des =="")return;
    QString last_date = "";
    //READ MESSAGES FROM LOCAL DATABASE TO GET LAST MESSAGE DATE
    try{
        QVector<DB::TableChats> list = DB::selectTblChats(m_type != PERSONAL_CHAT ? "*": m_username , m_des);
        for(auto it = list.begin();it!=list.end();it++){
            last_date =  QDateTime::fromString((*it).date , "yyyyMMddHHmmss").addSecs(1).toString("yyyyMMddHHmmss");
        }

    }  catch(QString &err){
        qDebug() << err;
    }

    //GET NEW MESSAGES FROM API
    RequestHandler *req_handler = new RequestHandler(this);
    connect(req_handler,&RequestHandler::errorOccured,[=](QString err){
        qDebug()<< "error fetching chat data " <<err;
    });
    connect(req_handler,&RequestHandler::dataReady,[=](QJsonObject jsonObj){
        QString code = jsonObj.value("code").toString();
        if (code == "200")
        {
            QString message = jsonObj.value("message").toString();
            int numberOfMessages = message.mid(message.indexOf("-")+1, message.lastIndexOf("-")-message.indexOf("-")-1).toInt();
            for (int i =0 ; i< numberOfMessages ; ++i) {

                QString key = "block " + QString::number(i);
                if (jsonObj.contains(key)) {
                    QJsonObject block = jsonObj.value(key).toObject();
                    QString body = block.value("body").toString();
                    QString src = block.value("src").toString();
                    QString dst = block.value("dst").toString();
                    QString date = block.value("date").toString();
                    //STORE MESAGE IN LOCAL DATABASE
                    try{
                        DB::insertTblChats(src ,dst , body , QDateTime::fromString(date , "yyyy-MM-dd HH:mm:ss").toString("yyyyMMddHHmmss"));
                    }catch(QString &err)
                    {
                        qDebug() << err;
                    }
                }
            }

        }
        else{
             QString message = jsonObj.value("message").toString();
            qDebug() << "error fetching chat data error code : " << code << "message : " << message;
        }
    });

    connect(req_handler,&RequestHandler::done,[=](){
        //READ MESSAGES FROM LOCAL DATABASE
        QVector<chatMsg> result;
        try{
            chatMsg msg_strcut;
            QVector<DB::TableChats> list = DB::selectTblChats(m_type != PERSONAL_CHAT ? "*": m_username , m_des);
            for(auto it = list.begin();it!=list.end();it++){
                msg_strcut.msg = (m_type == CHANNEL_CHAT ? (*it).dst : (*it).src) + " : " + (*it).body + "\n\n" + (QDateTime::fromString((*it).date, "yyyyMMddHHmmss").toString("yyyy-MM-dd HH:mm:ss"));
                if(m_username!=(*it).src || m_type == CHANNEL_CHAT)
                    msg_strcut.isRight = false;
                else
                    msg_strcut.isRight = true;
                result.push_back(msg_strcut);

            }

        }catch(QString &err){
            qDebug() << err;
        }
        emit chatsReady(result);
    });
    QString date_param = (last_date != "") ? ("&date=" + last_date) : "";
    req_handler->fetchData(QString(API_ADRESS)+ "/" + m_endpoint + "?token="+m_token+"&dst="+m_des+date_param);
}

void WorkerChat::sendChat()
{
    QString url;
    if(m_type == PERSONAL_CHAT){
        url = QString(API_ADRESS)+"/sendmessageuser?token="+m_token+"&dst="+m_des+"&body="+m_body;
    } else if(m_type == CHANNEL_CHAT){
        url = QString(API_ADRESS)+"/sendmessagechannel?token="+m_token+"&dst="+m_des+"&body="+m_body;
    } else if(m_type == GROUP_CHAT){
        url = QString(API_ADRESS)+"/sendmessagegroup?token="+m_token+"&dst="+m_des+"&body="+m_body;
    } else {
        qDebug() << "error sending message";
        return;
    }


    RequestHandler *req_handler = new RequestHandler(this);
    connect(req_handler,&RequestHandler::errorOccured,[=](QString err){
        qDebug()<< " error sending message : " <<  err;
        emit failedWrite();
    });
    connect(req_handler,&RequestHandler::dataReady,[=](QJsonObject jsonObj ){
        QString code = jsonObj.value("code").toString();
        if (code == "200") {
           emit chatSended();
        } else if(code == "404") {
            emit failedWrite();
            qDebug() << "404 error in sending message";
        } else {
             emit failedWrite();
            qDebug() << "error sending message";
        }
    });
    req_handler->fetchData(url);
}
