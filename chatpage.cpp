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
#include "addchat.h"
#include <QThread>
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
    //CREATE DATABASE TABLES IF NOT EXISTS
    try{
        DB::createTblChatsList();
        DB::createTblChats();
    }catch(QString &err){
         qDebug()<<err;
    }
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    ui->setupUi(this);

    //SET STYLESHEET OF WIDGET LISTS
    ui->messagesList_channel->setStyleSheet(message_list_styles.arg("messagesList_channel"));
    ui->messagesList_chat->setStyleSheet(message_list_styles.arg("messagesList_chat"));
    ui->messagesList_group->setStyleSheet(message_list_styles.arg("messagesList_group"));
    ui->chatsList->setStyleSheet(
        "QListWidget#chatsList{"
        "background-image: url(:/src/img/pattern-2.jpg);"
        "background-repeat: no-repeat;"
        "background-attachment: fixed;"
        "border:1px solid #324D6A;"
        "outline:none;"
        "padding-top:30px;"
        "padding-bottom:15px;"
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

    // CONFIG THE THREAD TO RUN REPEATEDLY TO UPDATE CHAT DATA
    m_workerThread  = new QThread;
    m_workerlist = new WorkerList(m_token);
    m_workerlist->moveToThread(m_workerThread);
    connect(m_workerThread, &QThread::finished, m_workerlist, &QObject::deleteLater);
    connect(m_workerThread, &QThread::started, m_workerlist, &WorkerList::openDB);
    connect(m_workerlist, &WorkerList::listUserReady,this , &ChatPage::handleUserListResult);
    connect(m_workerlist, &WorkerList::listChannelReady,this , &ChatPage::handleChannelListResult);
    connect(m_workerlist, &WorkerList::listGroupReady,this , &ChatPage::handleGroupListResult);
    connect(m_workerlist, &WorkerList::failed,this , &ChatPage::handleFailedListResult);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,m_workerlist , &WorkerList::getUserList);
    connect(timer, &QTimer::timeout,m_workerlist , &WorkerList::getChannelList);
    connect(timer, &QTimer::timeout,m_workerlist , &WorkerList::getGroupList);
    m_workerThread->start();
    timer->start(5000);

    //GET INITIAL DATA
    QTimer::singleShot(0,m_workerlist ,&WorkerList::getUserList);
    QTimer::singleShot(0,m_workerlist ,&WorkerList::getChannelList);
    QTimer::singleShot(0,m_workerlist ,&WorkerList::getGroupList);
    //ui->tabWidget->setCurrentIndex(0);


}

ChatPage::~ChatPage()
{
    m_workerThread->quit();
    m_workerThread->wait();
    delete m_workerThread;
    delete ui;
}

//A FUNCTION TO GET LIST OF CHATS

void ChatPage::getList(int chatType, QListWidget *listWidget, const QString &endpoint)
{
    listWidget->clear();
    //READ LIST FROM LOCAL DATABASE
    try {
    QVector<DB::TableChatsList> list = DB::selectTblChatsList(chatType);
    for (auto it = list.begin(); it != list.end(); it++)
        listWidget->addItem((*it).username);
    }
    catch (QString& err) {
    qDebug() << err;
    }

    if (m_selectedChatIndex!=-1 && currentTab == chatType-1 && currentTab == m_tabIndex)
    listWidget->setCurrentRow(m_selectedChatIndex);
    //GET LIST FROM API
    RequestHandler *req_handler = new RequestHandler(this);
    connect(req_handler,&RequestHandler::errorOccured,[=](QString err){
        ui->lbl_con_status->setText("you are offline");
        ui->lbl_con_status->setStyleSheet("background-color:rgba(255, 74, 74,0.6);color:white;font-weight:bold;padding:5px;");
        qDebug()<<err;
    });
    connect(req_handler,&RequestHandler::dataReady,[=](QJsonObject jsonObj){
        QString code = jsonObj.value("code").toString();
        ui->lbl_con_status->setText("you are online");
        ui->lbl_con_status->setStyleSheet("background-color:rgba(15, 184, 0,0.6);color:white;font-weight:bold;padding:5px;");
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
                    //INSERT DATA TO LOCAL DATABASE
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
        if (m_selectedChatIndex!=-1 && currentTab == chatType-1 && currentTab == m_tabIndex)
            listWidget->setCurrentRow(m_selectedChatIndex);
    });
    req_handler->fetchData(QString(API_ADRESS) + endpoint + "?token=" + m_token);
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

//A FUNCTION TO GET CHATS MESSAGES

void ChatPage::updateCurrentChatMessages()
{
    if(ui->chat_title->text()!= "updating ...")
    m_currentChatName = ui->chat_title->text();
    if(m_tabIndex==0)
    getUserChat(m_currentChatName);
    else if(m_tabIndex==1)
    getChannelChat(m_currentChatName);
    else if(m_tabIndex==2)
    getGroupChat(m_currentChatName);
}

void ChatPage::getChat(QString item , QString endpoint , int type){
    ui->chatsList->clear();
    QString last_date = "";
    //READ MESSAGES FROM LOCAL DATABASE
    try{
    QVector<DB::TableChats> list = DB::selectTblChats(type != PERSONAL_CHAT ? "*": m_username , item);
    for(auto it = list.begin();it!=list.end();it++){
        QString msg = (type == CHANNEL_CHAT ? (*it).dst : (*it).src) + " : " + (*it).body + "\n\n" + (QDateTime::fromString((*it).date, "yyyyMMddHHmmss").toString("yyyy-MM-dd HH:mm:ss"));
        QListWidgetItem *item = new QListWidgetItem(msg , ui->chatsList);
        if(m_username!=(*it).src || type == CHANNEL_CHAT)
            item->setTextAlignment(Qt::AlignLeft);
        else
            item->setTextAlignment(Qt::AlignRight);
        last_date =  QDateTime::fromString((*it).date , "yyyyMMddHHmmss").addSecs(2).toString("yyyyMMddHHmmss");
          }

     }  catch(QString &err){
           qDebug() << err;
        }
    ui->chat_title->setText("updating ...");
    QString des_user = item;

    //GET NEW MESSAGES FROM API
    RequestHandler *req_handler = new RequestHandler(this);
    connect(req_handler,&RequestHandler::errorOccured,[=](QString err){
        ui->chat_title->setText(des_user);
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
                QVector<DB::TableChats> list = DB::selectTblChats(type != PERSONAL_CHAT ? "*": m_username , item);
                for(auto it = list.begin();it!=list.end();it++){
                    QString msg = (type == CHANNEL_CHAT ? (*it).dst : (*it).src) + " : " + (*it).body + "\n\n" + (QDateTime::fromString((*it).date, "yyyyMMddHHmmss").toString("yyyy-MM-dd HH:mm:ss"));
                    QListWidgetItem *item = new QListWidgetItem(msg , ui->chatsList);
                    if(m_username!=(*it).src || type == CHANNEL_CHAT)
                          item->setTextAlignment(Qt::AlignLeft);
                    else
                        item->setTextAlignment(Qt::AlignRight);

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

void ChatPage::getUserChat(QString item)
{
    getChat(item,"getuserchats",PERSONAL_CHAT);
}

void ChatPage::getChannelChat(QString item)
{
    getChat(item,"getchannelchats",CHANNEL_CHAT);
}

void ChatPage::handleUserListResult(QVector<QString> result)
{
    /*if (m_selectedChatIndex!=-1 && currentTab == chatType-1 && currentTab == m_tabIndex)
           ui->messagesList_chat->setCurrentRow(m_selectedChatIndex);*/
    qDebug() << "thread updated!";
    ui->messagesList_chat->clear();
    //ADD Result to list widget
    for (auto it = result.begin(); it != result.end(); it++){

       ui->messagesList_chat->addItem((*it));

    }


    ui->tabWidget->setCurrentIndex(currentTab);

}
void ChatPage::handleChannelListResult(QVector<QString> result)
{


    /*if (m_selectedChatIndex!=-1 && currentTab == chatType-1 && currentTab == m_tabIndex)
           ui->messagesList_channel->setCurrentRow(m_selectedChatIndex);*/

    ui->messagesList_channel->clear();
    //ADD Result to list widget
    for (auto it = result.begin(); it != result.end(); it++){
      ui->messagesList_channel->addItem((*it));

    }


    ui->tabWidget->setCurrentIndex(currentTab);

}
void ChatPage::handleGroupListResult(QVector<QString> result)
{


    /*if (m_selectedChatIndex!=-1 && currentTab == chatType-1 && currentTab == m_tabIndex)
           ui->messagesList_group->setCurrentRow(m_selectedChatIndex);*/

    ui->messagesList_group->clear();
    //ADD Result to list widget
    for (auto it = result.begin(); it != result.end(); it++){
      ui->messagesList_group->addItem((*it));

    }


    ui->tabWidget->setCurrentIndex(currentTab);

}

void ChatPage::handleFailedListResult()
{

}

void ChatPage::getGroupChat(QString item)
{
    getChat(item,"getgroupchats",GROUP_CHAT);
}

void ChatPage::on_messagesList_chat_itemClicked(QListWidgetItem* item)
{
    m_selectedChatIndex = ui->messagesList_chat->currentRow();
    getUserChat(item->text());
    m_tabIndex = currentTab;
    m_currentChatName = item->text();
    ui->messagesList_channel->setCurrentRow(-1);
    ui->messagesList_group->setCurrentRow(-1);

}

void ChatPage::on_messagesList_channel_itemClicked(QListWidgetItem *item)
{
    m_selectedChatIndex = ui->messagesList_channel->currentRow();
    m_tabIndex = currentTab;
    m_currentChatName = item->text();
    getChannelChat(item->text());
    ui->messagesList_chat->setCurrentRow(-1);
    ui->messagesList_group->setCurrentRow(-1);

}

void ChatPage::on_messagesList_group_itemClicked(QListWidgetItem *item)
{
    m_selectedChatIndex = ui->messagesList_group->currentRow();
    m_tabIndex = currentTab;
    m_currentChatName = item->text();
    m_currentChatName = item->text();
    getGroupChat(item->text());
    ui->messagesList_channel->setCurrentRow(-1);
    ui->messagesList_chat->setCurrentRow(-1);

}

//A FUNCTION TO SEND MESSAGES

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


    RequestHandler *req_handler = new RequestHandler(this);
    connect(req_handler,&RequestHandler::errorOccured,[=](QString err){
        qDebug()<<err;
        ui->btn_sendMessage->setDisabled(false);
    });
    connect(req_handler,&RequestHandler::dataReady,[=](QJsonObject jsonObj ){
        QString code = jsonObj.value("code").toString();
        if (code == "200") {
            //INSERT NEW MESSAGE TO LOCAL DATABASE
            try {
                DB::insertTblChats(m_username ,dst , body ,date);
            } catch(QString &err){
                qDebug() << err;
            }
            QString msg;
            if(m_tabIndex!=1)
             msg = m_username + " : " + body + "\n\n" + QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd HH:mm:ss");
            else
             msg = dst + " : " + body + "\n\n" + QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd HH:mm:ss");

            QListWidgetItem *item = new QListWidgetItem(msg, ui->chatsList);
            if(m_tabIndex == 0 || m_tabIndex == 2){
                item->setTextAlignment(Qt::AlignRight);
            } else {
                item->setTextAlignment(Qt::AlignLeft);
            }
            ui->input_message->clear();
        } else if(code == "404") {
            ui->input_message->clear();
            QMessageBox::warning(this ,"error" ,"you can not send message to this user");
            qDebug() << "404 error in sending message";
        } else {
            qDebug() << "error";
        }
        ui->btn_sendMessage->setDisabled(false);
        ui->chatsList->scrollToBottom();
    });
    req_handler->fetchData(url);

}

void ChatPage::on_btn_sendMessage_clicked()
{
    if(ui->input_message->text() == "" || m_currentChatName== "") return;
    QString dst =m_currentChatName;
    QString body = ui->input_message->text();
    QString date = QDateTime::currentDateTimeUtc().toString("yyyyMMddHHmmss");
    sendChatMessage(dst, body, date);
}

//FUNCTIONS TO CREATE CHATS

void ChatPage::on_btn_new_chat_clicked()
{
    AddChat newChat(m_token,PERSONAL_CHAT,"/sendmessageuser");
    newChat.exec();
}

void ChatPage::on_btn_new_channel_clicked()
{
    AddChat newChannel(m_token,CHANNEL_CHAT,"/createchannel");
    newChannel.exec();
}

void ChatPage::on_btn_new_group_clicked()
{
    AddChat newGroup(m_token,GROUP_CHAT,"/creategroup");
    newGroup.exec();
}


//ANOTHER FUNCTIONS

void ChatPage::on_tabWidget_currentChanged(int index)
{
    currentTab = index ;
    if (index==0 && m_selectedChatIndex!=-1 && currentTab == m_tabIndex)
           ui->messagesList_chat->setCurrentRow(m_selectedChatIndex);
    else if (index==1 && m_selectedChatIndex!=-1 && currentTab == m_tabIndex)
           ui->messagesList_channel->setCurrentRow(m_selectedChatIndex);
    else if (index==2 && m_selectedChatIndex!=-1 && currentTab == m_tabIndex)
           ui->messagesList_group->setCurrentRow(m_selectedChatIndex);

    if(currentTab==0)
         QTimer::singleShot(0,m_workerlist ,&WorkerList::getUserList);
    else if(currentTab==1)
            QTimer::singleShot(0,m_workerlist ,&WorkerList::getChannelList);
    else
            QTimer::singleShot(0,m_workerlist ,&WorkerList::getGroupList);
}

void ChatPage::on_btn_scrollBottom_clicked()
{
    ui->chatsList->scrollToBottom();
}

void ChatPage::on_btn_logout_clicked()
{
    ui->btn_logout->setDisabled(true);
    RequestHandler *req_handler = new RequestHandler(this);
    connect(req_handler,&RequestHandler::errorOccured,[=](QString err){
        qDebug()<<err;
        QMessageBox::warning(this ,"error" ,"something went wrong");
        ui->btn_logout->setDisabled(false);
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
            //CLOSE THE CHAT PAGE
            this->accept();
        }
        else{
            QMessageBox::warning(this ,"error" ,message);
        }
        ui->btn_logout->setDisabled(false);

    });
    req_handler->fetchData(QString(API_ADRESS)+"/logout?username="+m_username+"&password="+m_password);
}

