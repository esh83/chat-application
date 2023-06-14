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
//left sidebar lists

void ChatPage::getUsersList()
{
    ui->messagesList_chat->clear();
    try{
         QVector<DB::TableChatsList> list = DB::selectTblChatsList(PERSONAL_CHAT);
         for(auto it = list.begin();it!=list.end();it++)
             ui->messagesList_chat->addItem((*it).username);
    }catch(QString &err){
         qDebug() << err;
    }

        if(m_tabIndex == 0)
            ui->messagesList_chat->setCurrentRow(m_selectedChatIndex);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url(QString(API_ADRESS)+"/getuserlist?token="+m_token);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() mutable
            {
                if (reply->error() != QNetworkReply::NoError)
                {
                    qDebug()<<"request error: " << reply->errorString();
                }
                else
                {
                    QByteArray response = reply->readAll();
                    qDebug()<<response;
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                    QJsonObject jsonObj = jsonDoc.object();
                    QString code = jsonDoc.object().value("code").toString();

                    if (code == "200")
                    {
                        ui->tabWidget->setCurrentIndex(m_tabIndex);
                        ui->messagesList_chat->clear();
                        DB::deleteTblChatsList(PERSONAL_CHAT);

                        for(auto it = jsonObj.begin() ; it != jsonObj.end(); it++){
                            if(it.value().isObject()){
                                QString username = it.value().toObject().value("src").toString();
                                try{
                                    DB::insertTblChatsList(username,"Jogn Doe",PERSONAL_CHAT);
                                }catch(QString &err){
                                    qDebug()<<err;
                                }


                                ui->messagesList_chat->addItem(username);
                            }

                        }
                    }
                    else{
                        qDebug() << "error";
                    }

                }
                if(m_tabIndex == 0)
                    ui->messagesList_chat->setCurrentRow(m_selectedChatIndex);
                reply->deleteLater();
            });

}

void ChatPage::getGroupList()
{

    ui->messagesList_group->clear();

    try{
            QVector<DB::TableChatsList> list = DB::selectTblChatsList(GROUP_CHAT);
            for(auto it = list.begin();it!=list.end();it++)
             ui->messagesList_group->addItem((*it).username);
    }catch(QString &err){
            qDebug() << err;
    }
    if(m_tabIndex == 2)
            ui->messagesList_group->setCurrentRow(m_selectedChatIndex);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url(QString(API_ADRESS)+"/getgrouplist?token="+m_token);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() mutable
            {
                if (reply->error() != QNetworkReply::NoError)
                {
                    qDebug()<<"request error: " << reply->errorString();
                }
                else
                {
                    QByteArray response = reply->readAll();
                    qDebug()<<response;
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                    QJsonObject jsonObj = jsonDoc.object();
                    QString code = jsonDoc.object().value("code").toString();

                    if (code == "200")
                    {
                        ui->tabWidget->setCurrentIndex(m_tabIndex);
                        ui->messagesList_group->clear();
                        DB::deleteTblChatsList(GROUP_CHAT);

                        for(auto it = jsonObj.begin() ; it != jsonObj.end(); it++){
                            if(it.value().isObject()){
                                QString groupName = it.value().toObject().value("group_name").toString();
                                try{
                                    DB::insertTblChatsList(groupName,"Jogn Doe",GROUP_CHAT);
                                }catch(QString &err){
                                    qDebug()<<err;
                                }
                                ui->messagesList_group->addItem(groupName);
                            }

                        }
                    }
                    else{
                        qDebug() << "error";
                    }

                }
                if(m_tabIndex == 2)
                ui->messagesList_group->setCurrentRow(m_selectedChatIndex);
                reply->deleteLater();
            });


}

void ChatPage::getChannelList()
{
    ui->messagesList_channel->clear();
    try{
            QVector<DB::TableChatsList> list = DB::selectTblChatsList(CHANNEL_CHAT);
            for(auto it = list.begin();it!=list.end();it++)
             ui->messagesList_channel->addItem((*it).username);
    }catch(QString &err){
            qDebug() << err;
    }
    if(m_tabIndex == 1)
            ui->messagesList_channel->setCurrentRow(m_selectedChatIndex);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url(QString(API_ADRESS)+"/getchannellist?token="+m_token);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() mutable
            {
                if (reply->error() != QNetworkReply::NoError)
                {
                    qDebug()<<"request error: " << reply->errorString();
                }
                else
                {
                    QByteArray response = reply->readAll();
                    qDebug()<<response;
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                    QJsonObject jsonObj = jsonDoc.object();
                    QString code = jsonDoc.object().value("code").toString();

                    if (code == "200")
                    {
                        ui->tabWidget->setCurrentIndex(m_tabIndex);
                        ui->messagesList_channel->clear();
                        DB::deleteTblChatsList(CHANNEL_CHAT);
                        for(auto it = jsonObj.begin() ; it != jsonObj.end(); it++){
                            if(it.value().isObject()){
                                QString channelName = it.value().toObject().value("channel_name").toString();
                                try{
                                    DB::insertTblChatsList(channelName,"Jogn Doe",CHANNEL_CHAT);
                                }catch(QString &err){
                                    qDebug()<<err;
                                }
                                ui->messagesList_channel->addItem(channelName);
                            }

                        }
                    }
                    else{
                        qDebug() << "error";
                    }

                }
                if(m_tabIndex == 1)
                    ui->messagesList_channel->setCurrentRow(m_selectedChatIndex);

                reply->deleteLater();
            });


}

void ChatPage::updateCurrentChatMessages()
{
    QString currentName = ui->chat_title->text();

    if(ui->tabWidget->currentIndex()==0)
    getUserChat(currentName);
    else if(ui->tabWidget->currentIndex()==1)
    getChannelChat(currentName);
    else if(ui->tabWidget->currentIndex()==2)
    getGroupChat(currentName);
}



void ChatPage::on_messagesList_chat_itemClicked(QListWidgetItem* item)
{

    QString m_dst = item->text();
    m_selectedChatIndex = ui->messagesList_chat->currentRow();
    getUserChat(m_dst);
    ui->messagesList_channel->setCurrentRow(-1);
     ui->messagesList_group->setCurrentRow(-1);

}

void ChatPage::getUserChat(QString item)
{

    ui->chat_title->setText("Loaing...");
    QString m_dst = item;
    //    QString m_date =NULL;

    ui->btn_sendMessage->show();
    ui->input_message->show();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url(QString(API_ADRESS)+"/getuserchats?token="+m_token+"&dst="+m_dst);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() mutable
            {
                if (reply->error() != QNetworkReply::NoError)
                {
                    qDebug()<<"request error: " << reply->errorString();
                }
                else
                {
                    QByteArray response = reply->readAll();
                    qDebug()<<response;
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                    QJsonObject jsonObj = jsonDoc.object();
                    QString code = jsonDoc.object().value("code").toString();

                    if (code == "200")
                    {
                        ui->chatsList->clear();
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
                                  int msgLength = body.length();
                                QString message = src + " : " + body + "\n\n" + date;


                                if(m_username!=src){
                                    QListWidgetItem *item = new QListWidgetItem(message , ui->chatsList);
                                    item->setTextAlignment(Qt::AlignLeft);
                                    item->setSizeHint(QSize(100 , 150));
                                }else{
                                    QListWidgetItem *item = new QListWidgetItem(message , ui->chatsList);
                                    item->setTextAlignment(Qt::AlignRight);
                                    item->setSizeHint(QSize(100 , 150));
                                }



                            }

                        }
                        ui->chat_title->setText(m_dst);

                    }
                    else{
                        qDebug() << "error";
                    }

                }
                reply->deleteLater();
            });


}


void ChatPage::on_messagesList_group_itemClicked(QListWidgetItem *item)
{
    QString m_dst = item->text();
     m_selectedChatIndex = ui->messagesList_group->currentRow();
   getGroupChat(m_dst);
     ui->messagesList_channel->setCurrentRow(-1);
     ui->messagesList_chat->setCurrentRow(-1);

}

void ChatPage::getGroupChat(QString item)
{

    ui->chat_title->setText("Loaing...");
    QString m_dst = item;
    //    QString m_date =NULL;
    ui->btn_sendMessage->show();
    ui->input_message->show();


    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url(QString(API_ADRESS)+"/getgroupchats?token="+m_token+"&dst="+m_dst);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() mutable
            {
                if (reply->error() != QNetworkReply::NoError)
                {
                    qDebug()<<"request error: " << reply->errorString();
                }
                else
                {
                    QByteArray response = reply->readAll();
                    qDebug()<<response;
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                    QJsonObject jsonObj = jsonDoc.object();
                    QString code = jsonDoc.object().value("code").toString();

                    if (code == "200")
                    {
                          ui->chatsList->clear();
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
                                 int msgLength = body.length();
                                QString message = src + " : " + body +"\n\n" + date ;



                                if(m_username!=src){
                                    QListWidgetItem *item = new QListWidgetItem(message , ui->chatsList);
                                    item->setTextAlignment(Qt::AlignLeft);
                                    item->setSizeHint(QSize(100,150));
                                }else{
                                    QListWidgetItem *item = new QListWidgetItem(message , ui->chatsList);
                                    item->setTextAlignment(Qt::AlignRight);
                                    item->setSizeHint(QSize(100 , 150));
                                }


                            }

                        }
                        ui->chat_title->setText(m_dst);

                    }
                    else{
                        qDebug() << "error";
                    }

                }
                reply->deleteLater();
            });


}


void ChatPage::on_messagesList_channel_itemClicked(QListWidgetItem *item)
{
    QString m_dst = item->text();
     m_selectedChatIndex = ui->messagesList_channel->currentRow();
    getChannelChat(m_dst);
     ui->messagesList_chat->setCurrentRow(-1);
     ui->messagesList_group->setCurrentRow(-1);

}

void ChatPage::getChannelChat(QString item)
{

    ui->chat_title->setText("Loaing...");
    QString m_dst = item;
    //    QString m_date =NULL;
    ui->btn_sendMessage->show();
    ui->input_message->show();


    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url(QString(API_ADRESS)+"/getchannelchats?token="+m_token+"&dst="+m_dst);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() mutable
            {
                if (reply->error() != QNetworkReply::NoError)
                {
                    qDebug()<<"request error: " << reply->errorString();
                }
                else
                {
                    QByteArray response = reply->readAll();
                    qDebug()<<response;
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                    QJsonObject jsonObj = jsonDoc.object();
                    QString code = jsonDoc.object().value("code").toString();

                    if (code == "200")
                    {
                        ui->chatsList->clear();
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
                                  int msgLength = body.length();
                                QString message = m_dst + " : " + body + "\n\n" + date ;
                                QListWidgetItem *item = new QListWidgetItem(message , ui->chatsList);
                                item->setTextAlignment(Qt::AlignLeft);
                                item->setSizeHint(QSize(100 , 150));



                            }
                            ui->chat_title->setText(m_dst);

                        }

                    }
                    else{
                        qDebug() << "error";
                    }

                }
                reply->deleteLater();
            });


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
    if(ui->input_message->text() == "")return;
    ui->btn_sendMessage->setDisabled(true);

    if(m_tabIndex==0)
    {

        QString m_dst = ui->chat_title->text();
        QString m_body = ui->input_message->text();

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QUrl url(QString(API_ADRESS)+"/sendmessageuser?token="+m_token+"&dst="+m_dst+"&body="+m_body);
        QNetworkRequest request(url);
        QNetworkReply* reply = manager->get(request);

        connect(reply, &QNetworkReply::finished, [=]() mutable
                {
                    if (reply->error() != QNetworkReply::NoError)
                    {
                        qDebug()<<"request error: " << reply->errorString();
                    }
                    else
                    {
                        QByteArray response = reply->readAll();
                        qDebug()<<response;
                        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                        QJsonObject jsonObj = jsonDoc.object();
                        QString code = jsonDoc.object().value("code").toString();

                        if (code == "200")
                        {
                            QListWidgetItem *item = new QListWidgetItem( m_body , ui->chatsList);
                            item->setTextAlignment(Qt::AlignRight);
                            item->setSizeHint(QSize(100 , 100));
                            ui->input_message->clear();

                        }
                        else{
                            qDebug() << "error";
                        }

                    }

                    ui->btn_sendMessage->setDisabled(false);
                    ui->chatsList->scrollToBottom();
                    reply->deleteLater();
                });


    }
    else if(m_tabIndex==1)
    {

        QString m_dst = ui->chat_title->text();
        QString m_body = ui->input_message->text();

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QUrl url(QString(API_ADRESS)+"/sendmessagechannel?token="+m_token+"&dst="+m_dst+"&body="+m_body);
        QNetworkRequest request(url);
        QNetworkReply* reply = manager->get(request);

        connect(reply, &QNetworkReply::finished, [=]() mutable
                {
                    if (reply->error() != QNetworkReply::NoError)
                    {
                        qDebug()<<"request error: " << reply->errorString();
                    }
                    else
                    {
                        QByteArray response = reply->readAll();
                        qDebug()<<response;
                        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                        QJsonObject jsonObj = jsonDoc.object();
                        QString code = jsonDoc.object().value("code").toString();

                        if (code == "200")
                        {
                            QListWidgetItem *item = new QListWidgetItem( m_body , ui->chatsList);
                            item->setTextAlignment(Qt::AlignLeft);
                            item->setSizeHint(QSize(100 , 100));
                            ui->input_message->clear();

                        }
                        else if(code == "404"){
                            ui->btn_sendMessage->hide();
                            ui->input_message->hide();
                            ui->input_message->clear();
                            QMessageBox::warning(this ,"error" ,"you are not admin in this channel");
                            qDebug() << "Admin error";
                        }
                        else
                            qDebug() << "error";


                    }

                    ui->btn_sendMessage->setDisabled(false);
                    ui->chatsList->scrollToBottom();
                    reply->deleteLater();
                });


    }
    else if(m_tabIndex==2)
    {


            QString m_dst = ui->chat_title->text();
            QString m_body = ui->input_message->text();

            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            QUrl url(QString(API_ADRESS)+"/sendmessagegroup?token="+m_token+"&dst="+m_dst+"&body="+m_body);
            QNetworkRequest request(url);
            QNetworkReply* reply = manager->get(request);

            connect(reply, &QNetworkReply::finished, [=]() mutable
                    {
                        if (reply->error() != QNetworkReply::NoError)
                        {
                            qDebug()<<"request error: " << reply->errorString();
                        }
                        else
                        {
                            QByteArray response = reply->readAll();
                            qDebug()<<response;
                            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                            QJsonObject jsonObj = jsonDoc.object();
                            QString code = jsonDoc.object().value("code").toString();

                            if (code == "200")
                            {
                                QListWidgetItem *item = new QListWidgetItem( m_body , ui->chatsList);
                                item->setTextAlignment(Qt::AlignRight);
                                item->setSizeHint(QSize(100 , 100));
                                ui->input_message->clear();

                            }
                            else{
                                qDebug() << "error";
                            }

                        }

                        ui->btn_sendMessage->setDisabled(false);
                        ui->chatsList->scrollToBottom();
                        reply->deleteLater();
                    });


        }
        else
        {
            QMessageBox::warning(this ,"error" ,"Error sending message");
            qDebug() << "error";
        }



}






void ChatPage::on_tabWidget_currentChanged(int index)
{
    m_tabIndex = index ;
}


void ChatPage::on_btn_scrollBottom_clicked()
{
    ui->chatsList->scrollToBottom();
}

