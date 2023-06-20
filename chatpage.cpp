#include "chatpage.h"
#include "ui_chatpage.h"
#include <QSize>
#include <QBrush>
#include <QColor>
#include <QMessageBox>
#include <QShortcut>
#include "queries.h"
#include <QTimer>
#include <QVector>
#include <QDateTime>

QString message_list_styles = "QListWidget#%1{"
                  "background-color:white;"
                  "border:none;"
                  "outline:none;"
                  "border-radius:20px;"
                  "padding:20px 0px"
                  "}"
                  "QListWidget#%1::item {"
                  "color:black;"
                  "padding:5px;"
                  "height:40px;"
                  "border-bottom:1px solid lightgray;"

                  "}"
                  "QListWidget#%1::item:selected {"
                  "background-color: #324D6A;"
                  "color:white;"
                  "}";

ChatPage::ChatPage(QString password ,QString username, QString token , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatPage) , m_token(token) , m_username(username) , m_password(password), m_tabIndex(0)
{
    try{
         DB::createTblChatsList();
        DB::createTblChats();
    }catch(QString &err){
         qDebug()<<err;
    }



    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    ui->setupUi(this);

    getUsersList();
    getGroupList();
    getChannelList();
    ui->tabWidget->setCurrentIndex(0);
    ui->messagesList_chat->setCurrentRow(0);
    if(ui->messagesList_chat->currentItem())
    on_messagesList_chat_itemClicked(ui->messagesList_chat->currentItem());
    ui->messagesList_channel->setStyleSheet(message_list_styles.arg("messagesList_channel"));
    ui->messagesList_chat->setStyleSheet(message_list_styles.arg("messagesList_chat"));
    ui->messagesList_group->setStyleSheet(message_list_styles.arg("messagesList_group"));
    ui->chatsList->setStyleSheet(
        "QListWidget#chatsList{"
        "background-image: url(:/src/img/pattern-2.jpg);"
        " background-repeat: no-repeat;"
        "background-attachment: fixed;"
        "border:1px solid #324D6A;"
        "outline:none;"

        "padding:20px 0px"
        "}"
        "QListWidget#chatsList::item {"
        "padding:20px;"
        "color:black ;"
        " background-color: rgba(255,255,255,0.5);"
        "border-radius:10px;"
        "margin-top:10px;"
        "margin-bottom:10px;"
        "margin-left:20px;"
        "margin-right:20px;"
        "}"

        );


    m_updateThread = new UpdateThread(this);
    connect(m_updateThread, &UpdateThread::updateUsersList, this, &ChatPage::getUsersList);
    connect(m_updateThread, &UpdateThread::updateGroupList, this, &ChatPage::getGroupList);
    connect(m_updateThread, &UpdateThread::updateChannelList, this, &ChatPage::getChannelList);
    connect(m_updateThread, &UpdateThread::updateCurrentChatMessages, this, &ChatPage::updateCurrentChatMessages);
    m_updateThread->start();


}

ChatPage::~ChatPage()
{
    m_updateThread->quit();
    m_updateThread->wait();
    delete m_updateThread;
    delete ui;
}

//A FUNCTION TO GET LIST OF CHATS
void ChatPage::getList(int chatType, QListWidget *listWidget, const QString &endpoint)
{


    listWidget->clear();
    try {
    QVector<DB::TableChatsList> list = DB::selectTblChatsList(chatType);
    for (auto it = list.begin(); it != list.end(); it++)
        listWidget->addItem((*it).username);
    }
    catch (QString& err) {
    qDebug() << err;
    }

    if (currentTab == chatType-1 && currentTab == m_tabIndex)
    listWidget->setCurrentRow(m_selectedChatIndex);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QUrl url(QString(API_ADRESS) + endpoint + "?token=" + m_token);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() mutable
            {
                if (reply->error() != QNetworkReply::NoError)
                {
                    ui->lbl_con_status->setText("you are offline");
                    ui->lbl_con_status->setStyleSheet("background-color:rgba(255, 74, 74,0.6);color:white;font-weight:bold;padding:5px;");
                    qDebug() << "request error: " << reply->errorString();
                }
                else
                {
                    ui->lbl_con_status->setText("you are online");
                    ui->lbl_con_status->setStyleSheet("background-color:rgba(15, 184, 0,0.6);color:white;font-weight:bold;padding:5px;");
                    QByteArray response = reply->readAll();
                    qDebug() << response;
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                    QJsonObject jsonObj = jsonDoc.object();
                    QString code = jsonDoc.object().value("code").toString();

                    if (code == "200")
                    {
                        ui->tabWidget->setCurrentIndex(currentTab);
                        listWidget->clear();
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

                                try {
                                    DB::insertTblChatsList(name, "Jogn Doe", chatType);
                                }
                                catch (QString& err) {
                                    qDebug() << err;
                                }

                                listWidget->addItem(name);
                            }
                        }
                    }
                    else {
                        qDebug() << "error";
                    }

                }
                if (currentTab == chatType-1 && currentTab == m_tabIndex)
                    listWidget->setCurrentRow(m_selectedChatIndex);

                reply->deleteLater();
            });


}



void ChatPage::getUsersList()
{
    getList(PERSONAL_CHAT, ui->messagesList_chat, "/getuserlist");

}

void ChatPage::getGroupList()
{

    getList(GROUP_CHAT, ui->messagesList_group, "/getgrouplist");


}

void ChatPage::getChannelList()
{
    getList(CHANNEL_CHAT, ui->messagesList_channel, "/getchannellist");

}

void ChatPage::updateCurrentChatMessages()
{
    m_currentChatName = ui->chat_title->text();
    if(m_tabIndex==0)
    getUserChat(m_currentChatName);
    else if(m_tabIndex==1)
    getChannelChat(m_currentChatName);
    else if(m_tabIndex==2)
    getGroupChat(m_currentChatName);
}

//A FUNCTION TO GET CHATS MESSAGES
void ChatPage::getChat(QString item , QString endpoint , int type){
    ui->chatsList->clear();
    QString last_date = "";
    //READ MESSAGES FROM LOCAL DATABASE
    try{
    QVector<DB::TableChats> list = DB::selectTblChats(type == CHANNEL_CHAT ? "*": m_username , item);
    for(auto it = list.begin();it!=list.end();it++){
        QString msg = (type == CHANNEL_CHAT ? (*it).dst : (*it).src) + " : " + (*it).body + "\n\n" + (QDateTime::fromString((*it).date, "yyyyMMddHHmmss").toString("yyyy-MM-dd HH:mm:ss"));
        QListWidgetItem *item = new QListWidgetItem(msg , ui->chatsList);
        if(m_username!=(*it).src)
            item->setTextAlignment(Qt::AlignLeft);
        else
            item->setTextAlignment(Qt::AlignRight);
        item->setSizeHint(QSize(100 , 150));
        last_date = (*it).date;
    }
    }catch(QString &err){
    qDebug() << err;
    }



    ui->chat_title->setText("Loaing...");
    QString des_user = item;

    ui->btn_sendMessage->show();
    ui->input_message->show();

    //GET NEW MESSAGES FROM API
    RequestHandler *req_handler = new RequestHandler(this);
    connect(req_handler,&RequestHandler::errorOccured,[=](QString err){
        qDebug()<<err;
    });
    connect(req_handler,&RequestHandler::dataReady,[=](QJsonObject jsonObj){
         ui->chat_title->setText(des_user);
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

            ui->chatsList->clear();
                //READ MESSAGES FROM LOCAL DATABASE
            try{
                QVector<DB::TableChats> list = DB::selectTblChats(type == CHANNEL_CHAT ? "*": m_username , item);
                for(auto it = list.begin();it!=list.end();it++){
                    QString msg = (type == CHANNEL_CHAT ? (*it).dst : (*it).src) + " : " + (*it).body + "\n\n" + (QDateTime::fromString((*it).date, "yyyyMMddHHmmss").toString("yyyy-MM-dd HH:mm:ss"));
                    QListWidgetItem *item = new QListWidgetItem(msg , ui->chatsList);
                    if(m_username!=(*it).src)
                        item->setTextAlignment(Qt::AlignLeft);
                    else
                        item->setTextAlignment(Qt::AlignRight);
                    item->setSizeHint(QSize(100 , 150));

                }

            }catch(QString &err){
                qDebug() << err;
            }

        }
        else{
            qDebug() << "error code : "+ code;
        }
    });
    QString date_param = (last_date != "") ? ("&date=" + last_date) : "";
    req_handler->fetchData(QString(API_ADRESS)+ "/" + endpoint + "?token="+m_token+"&dst="+des_user+date_param);


}

void ChatPage::on_messagesList_chat_itemClicked(QListWidgetItem* item)
{

    QString m_dst = item->text();
    m_selectedChatIndex = ui->messagesList_chat->currentRow();
    getUserChat(m_dst);
    m_tabIndex = currentTab;
    ui->messagesList_channel->setCurrentRow(-1);
    ui->messagesList_group->setCurrentRow(-1);

}


void ChatPage::getUserChat(QString item)
{
    getChat(item,"getuserchats",PERSONAL_CHAT);
}


void ChatPage::on_messagesList_group_itemClicked(QListWidgetItem *item)
{
    QString m_dst = item->text();
    m_selectedChatIndex = ui->messagesList_group->currentRow();
    m_tabIndex = currentTab;
    getGroupChat(m_dst);
    ui->messagesList_channel->setCurrentRow(-1);
    ui->messagesList_chat->setCurrentRow(-1);

}

void ChatPage::getGroupChat(QString item)
{
  getChat(item,"getgroupchats",GROUP_CHAT);
}


void ChatPage::on_messagesList_channel_itemClicked(QListWidgetItem *item)
{
    QString m_dst = item->text();
    m_selectedChatIndex = ui->messagesList_channel->currentRow();
    m_tabIndex = currentTab;
    getChannelChat(m_dst);
    ui->messagesList_chat->setCurrentRow(-1);
    ui->messagesList_group->setCurrentRow(-1);

}

void ChatPage::getChannelChat(QString item)
{
   getChat(item,"getchannelchats",CHANNEL_CHAT);

}






void ChatPage::on_btn_logout_clicked()
{



    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url(QString(API_ADRESS)+"/logout?username="+m_username+"&password="+m_password);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);

    ui->btn_logout->setDisabled(true);

    connect(reply, &QNetworkReply::finished, [=]() mutable
            {
                if (reply->error() != QNetworkReply::NoError)
                {
                    QMessageBox::warning(this ,"error" ,"something went wrong");
                    qDebug()<<"logout Error: " << reply->errorString();
                }
                else
                {
                    QByteArray response = reply->readAll();
                    qDebug()<<response;
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                    QJsonObject jsonObj = jsonDoc.object();
                   QString message = jsonObj.value("message").toString();
                   QString code = jsonObj.value("code").toString();


                    if (code == "200")
                    {
                       DB::emptyTblInfo();
                        DB::emptyTblChatsList();
                       DB::emptyTblChats();
                        this->accept();

                    }

                    else{
                        QMessageBox::warning(this ,"error" ,message);
                    }

                }
                ui->btn_logout->setText("Login");
                ui->btn_logout->setDisabled(false);
                reply->deleteLater();
            });



}


void ChatPage::on_btn_sendMessage_clicked()
{
    if(ui->input_message->text() == "") return;

    QString dst = ui->chat_title->text();
    QString body = ui->input_message->text();
    QString date = QDateTime::currentDateTimeUtc().toString("yyyyMMddHHmmss");

    sendChatMessage(dst, body, date);


}


void ChatPage::sendChatMessage(QString dst, QString body, QString date)
{

            ui->btn_sendMessage->setDisabled(true);

            QString url;
            if(m_tabIndex == 0){
             url = QString(API_ADRESS)+"/sendmessageuser?token="+m_token+"&dst="+dst+"&body="+body;
            } else if(m_tabIndex == 1){
             url = QString(API_ADRESS)+"/sendmessagechannel?token="+m_token+"&dst="+dst+"&body="+body;
            } else if(m_tabIndex == 2){
             url = QString(API_ADRESS)+"/sendmessagegroup?token="+m_token+"&dst="+dst+"&body="+body;
            } else {
             QMessageBox::warning(this ,"error" ,"Error sending message");
             qDebug() << "error";
             return;
            }

            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            QUrl api_url(url);
            QNetworkRequest request(api_url);
            QNetworkReply* reply = manager->get(request);

            connect(reply, &QNetworkReply::finished, [=]() mutable {
                if (reply->error() != QNetworkReply::NoError) {
                    qDebug()<<"request error: " << reply->errorString();
                } else {
                    QByteArray response = reply->readAll();
                    qDebug()<<response;
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                    QJsonObject jsonObj = jsonDoc.object();
                    QString code = jsonDoc.object().value("code").toString();

                    if (code == "200") {
                        try {
                            DB::insertTblChats(m_username ,dst , body ,date);
                        } catch(QString &err){
                            qDebug() << err;
                        }

                        QString msg = m_username + " : " + body + "\n\n" + QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd HH:mm:ss");
                        QListWidgetItem *item = new QListWidgetItem(msg, ui->chatsList);
                        item->setSizeHint(QSize(100 , 150));
                        if(m_tabIndex == 0 || m_tabIndex == 2){
                            item->setTextAlignment(Qt::AlignRight);
                        } else {
                            item->setTextAlignment(Qt::AlignLeft);
                        }
                        ui->input_message->clear();
                    } else if(code == "404") {
                        ui->btn_sendMessage->hide();
                        ui->input_message->hide();
                        ui->input_message->clear();
                        QMessageBox::warning(this ,"error" ,"you are not admin in this channel");
                        qDebug() << "Admin error";
                    } else {
                        qDebug() << "error";
                    }
                }

                ui->btn_sendMessage->setDisabled(false);
                ui->chatsList->scrollToBottom();

                reply->deleteLater();
            });


}



void ChatPage::on_tabWidget_currentChanged(int index)
{
            currentTab = index ;
}


void ChatPage::on_btn_scrollBottom_clicked()
{
            ui->chatsList->scrollToBottom();
}
