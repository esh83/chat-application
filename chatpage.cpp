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
#include <QMovie>
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
    m_workerchat = new WorkerChat(m_token);
    m_workerchat->m_endpoint = "getuserchats";
    m_workerchat->m_type = PERSONAL_CHAT;
    m_workerchat->m_username = m_username;
    m_workerchat->m_des = "";
    m_workerlist->moveToThread(m_workerThread);
    m_workerchat->moveToThread(m_workerThread);
    connect(m_workerThread, &QThread::finished, m_workerlist, &QObject::deleteLater);
    connect(m_workerThread, &QThread::finished, m_workerchat, &QObject::deleteLater);
    connect(m_workerThread, &QThread::started, m_workerlist, &WorkerList::openDB);
    //WORKER LIST SIGNALS AND SLOTS
    connect(m_workerlist, &WorkerList::listUserReady,this , &ChatPage::handleUserListResult);
    connect(m_workerlist, &WorkerList::listChannelReady,this , &ChatPage::handleChannelListResult);
    connect(m_workerlist, &WorkerList::listGroupReady,this , &ChatPage::handleGroupListResult);
    connect(m_workerlist, &WorkerList::failed,this , &ChatPage::handleFailedListResult);
       connect(m_workerlist, &WorkerList::success,this , &ChatPage::handleSuccessListResult);
     //WORKER CHAT SIGNALS AND SLOTS
     connect(m_workerchat , &WorkerChat::chatsReady,this , &ChatPage::handleChatResult);
     connect(m_workerchat , &WorkerChat::chatSended,this , &ChatPage::handleChatSended);
     connect(m_workerchat , &WorkerChat::failedWrite,this , &ChatPage::handleSendingFailed);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,m_workerlist , &WorkerList::getUserList);
    connect(timer, &QTimer::timeout,m_workerlist , &WorkerList::getChannelList);
    connect(timer, &QTimer::timeout,m_workerlist , &WorkerList::getGroupList);
    connect(timer, &QTimer::timeout,m_workerchat , &WorkerChat::getChats);
    m_workerThread->start();
    timer->start(15000);

    //GET INITIAL DATA
    QTimer::singleShot(0,m_workerlist ,&WorkerList::getUserList);
    QTimer::singleShot(0,m_workerlist ,&WorkerList::getChannelList);
    QTimer::singleShot(0,m_workerlist ,&WorkerList::getGroupList);
}

ChatPage::~ChatPage()
{
    m_workerThread->quit();
    m_workerThread->wait();
    delete m_workerThread;
    delete ui;
}
void ChatPage::handleUserListResult(QVector<QString> result)
{
    qDebug() << "thread updated!";
    ui->messagesList_chat->clear();
    //ADD Result to list widget
    for (auto it = result.begin(); it != result.end(); it++){
       ui->messagesList_chat->addItem((*it));
    }
    if(m_selectedChatIndex!=-1 && currentTab == 0 && currentTab == m_tabIndex)
       ui->messagesList_chat->setCurrentRow(m_selectedChatIndex);
    ui->tabWidget->setCurrentIndex(currentTab);

}
void ChatPage::handleChannelListResult(QVector<QString> result)
{


    ui->messagesList_channel->clear();
    //ADD Result to list widget
    for (auto it = result.begin(); it != result.end(); it++){
      ui->messagesList_channel->addItem((*it));

    }
    if(m_selectedChatIndex!=-1 && currentTab == 1 && currentTab == m_tabIndex)
      ui->messagesList_channel->setCurrentRow(m_selectedChatIndex);
    ui->tabWidget->setCurrentIndex(currentTab);

}
void ChatPage::handleGroupListResult(QVector<QString> result)
{

    ui->messagesList_group->clear();
    //ADD Result to list widget
    for (auto it = result.begin(); it != result.end(); it++){
      ui->messagesList_group->addItem((*it));
    }
    if(m_selectedChatIndex!=-1 && currentTab == 2 && currentTab == m_tabIndex)
      ui->messagesList_group->setCurrentRow(m_selectedChatIndex);
    ui->tabWidget->setCurrentIndex(currentTab);
}

void ChatPage::handleFailedListResult()
{
    ui->lbl_con_status->setStyleSheet("background-color:rgba(255, 74, 74,0.6);color:white;font-weight:bold;padding:5px;");
    ui->lbl_con_status->setText("you are offline");
}

void ChatPage::handleSuccessListResult()
{
    ui->lbl_con_status->setStyleSheet("background-color:rgba(15, 184, 0,0.6);color:white;font-weight:bold;padding:5px;");
    ui->lbl_con_status->setText("you are online");
}

void ChatPage::handleChatResult(QVector<chatMsg> result)
{
    if(result.length() > m_chats_count){
      m_shoud_scroll = true;
    }
    m_chats_count = result.length();
    ui->chat_title->setText(m_currentChatName);
    ui->chatsList->clear();
    for(auto it = result.begin();  it !=result.end(); it++){
      QListWidgetItem *item = new QListWidgetItem((*it).msg , ui->chatsList);
      if((*it).isRight)
        item->setTextAlignment(Qt::AlignRight);
      else
        item->setTextAlignment(Qt::AlignLeft);
    }
    if(m_shoud_scroll)
      ui->chatsList->scrollToBottom();
    m_shoud_scroll = false;
}

void ChatPage::handleChatSended()
{
     ui->btn_sendMessage->setIcon(QIcon(":/src/img/send_icon.png"));
    ui->btn_sendMessage->setDisabled(false);
    ui->chat_title->setText("loading ...");
    QTimer::singleShot(0,m_workerchat ,&WorkerChat::getChats);
    ui->input_message->clear();
}

void ChatPage::handleSendingFailed()
{
    ui->btn_sendMessage->setIcon(QIcon(":/src/img/send_icon.png"));
    ui->btn_sendMessage->setDisabled(false);
    QMessageBox::warning(this ,"Error" , "can not send your message");

}


void ChatPage::on_messagesList_chat_itemClicked(QListWidgetItem* item)
{
    m_chats_count = 0;
    m_selectedChatIndex = ui->messagesList_chat->currentRow();
    m_tabIndex = currentTab;
    m_currentChatName = item->text();
    ui->messagesList_channel->setCurrentRow(-1);
    ui->messagesList_group->setCurrentRow(-1);
    m_workerchat->m_des = item->text();
    m_workerchat->m_type = PERSONAL_CHAT;
    m_workerchat->m_endpoint = "getuserchats";
    ui->chatsList->clear();
    ui->chat_title->setText("loading ...");
     QTimer::singleShot(0,m_workerchat ,&WorkerChat::getChats);

}

void ChatPage::on_messagesList_channel_itemClicked(QListWidgetItem *item)
{
      m_chats_count = 0;
     m_selectedChatIndex = ui->messagesList_channel->currentRow();
    m_tabIndex = currentTab;
    m_currentChatName = item->text();
    ui->messagesList_chat->setCurrentRow(-1);
    ui->messagesList_group->setCurrentRow(-1);
    m_workerchat->m_des = item->text();
    m_workerchat->m_type = CHANNEL_CHAT;
    m_workerchat->m_endpoint = "getchannelchats";
    ui->chatsList->clear();
    ui->chat_title->setText("loading ...");
    QTimer::singleShot(0,m_workerchat ,&WorkerChat::getChats);

}

void ChatPage::on_messagesList_group_itemClicked(QListWidgetItem *item)
{
     m_chats_count = 0;
    m_selectedChatIndex = ui->messagesList_group->currentRow();
    m_tabIndex = currentTab;
    m_currentChatName = item->text();
    ui->messagesList_channel->setCurrentRow(-1);
    ui->messagesList_chat->setCurrentRow(-1);
    m_workerchat->m_des = item->text();
    m_workerchat->m_type = GROUP_CHAT;
    m_workerchat->m_endpoint = "getgroupchats";
    ui->chatsList->clear();
    ui->chat_title->setText("loading ...");
    QTimer::singleShot(0,m_workerchat ,&WorkerChat::getChats);

}

void ChatPage::on_btn_sendMessage_clicked()
{
    if(ui->input_message->text() == "" || m_currentChatName== "") return;
    QString body = ui->input_message->text();
    m_workerchat->m_body = body;
    ui->btn_sendMessage->setDisabled(true);
    QMovie * movie = new QMovie(this);
    movie->setFileName(":/src/img/loading_icon.gif");
    connect(movie, &QMovie::frameChanged, [=]{
        ui->btn_sendMessage->setIcon(movie->currentPixmap());
    });
    connect(m_workerchat , &WorkerChat::chatSended , movie , &QMovie::deleteLater);
    connect(m_workerchat , &WorkerChat::failedWrite , movie , &QMovie::deleteLater);
    movie->start();
    m_shoud_scroll = true;
    QTimer::singleShot(0,m_workerchat ,&WorkerChat::sendChat);
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

    if(currentTab==0){
           QTimer::singleShot(0,m_workerlist ,&WorkerList::getUserList);
    }

    else if(currentTab==1){ 
             QTimer::singleShot(0,m_workerlist ,&WorkerList::getChannelList);
    }

    else{
               QTimer::singleShot(0,m_workerlist ,&WorkerList::getGroupList);
    }

}

void ChatPage::on_btn_scrollBottom_clicked()
{
    ui->chatsList->scrollToBottom();
}

void ChatPage::on_btn_logout_clicked()
{
    ui->btn_logout->setDisabled(true);
    QMovie * movie = new QMovie(this);
    movie->setFileName(":/src/img/loading_icon.gif");
    connect(movie, &QMovie::frameChanged, [=]{
        ui->btn_logout->setIcon(movie->currentPixmap());
    });
    movie->start();
    RequestHandler *req_handler = new RequestHandler(this);
    connect(req_handler,&RequestHandler::errorOccured,[=](QString err){
        qDebug()<<err;
        QMessageBox::warning(this ,"error" ,"something went wrong");
        ui->btn_logout->setDisabled(false);
        delete movie;
        ui->btn_logout->setIcon(QIcon(":/src/img/out.png"));
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
        delete movie;
        ui->btn_logout->setIcon(QIcon(":/src/img/out.png"));

    });
    req_handler->fetchData(QString(API_ADRESS)+"/logout?username="+m_username+"&password="+m_password);
}


void ChatPage::on_btn_join_channel_clicked()
{
    AddChat newChannel(m_token,CHANNEL_CHAT,"/joinchannel");
    newChannel.exec();
}


void ChatPage::on_btn_join_group_clicked()
{
    AddChat newChannel(m_token,GROUP_CHAT,"/joingroup");
    newChannel.exec();
}

