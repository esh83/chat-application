#include "chatpage.h"
#include "ui_chatpage.h"
#include <QSize>
#include <QBrush>
#include <QColor>
ChatPage::ChatPage( QString token , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatPage) , m_token(token)
{
    getUsersList();
    getGroupList();
    getChannelList();

    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    ui->setupUi(this);

    ui->messagesList->setStyleSheet(
        "QListWidget#messagesList{"
        "background-color:white;"
        "border:none;"
        "outline:none;"
        "border-radius:20px;"
        "padding:20px 0px"
        "}"
        "QListWidget#messagesList::item {"
        "color:black;"
        "padding:5px;"
        "height:40px;"
        "border-bottom:1px solid lightgray;"

        "}"
        "QListWidget#messagesList::item:selected {"
        "background-color: #324D6A;"
        "color:white;"
        "}");
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
    /*for(int i =0 ; i < 10; i++){
        QListWidgetItem *item = new QListWidgetItem(QString("Digital Marketing GP %1\nvahid azari : hello guys whats up dudes how you doing").arg(QString::number(i)) , ui->messagesList);
        item->setTextAlignment(Qt::AlignLeft);

    }*/
    for(int i =0 ; i < 10; i++){
        if(i%2){
            QListWidgetItem *item = new QListWidgetItem(QString("ehsan shafiee : hello dude how are you ?") , ui->chatsList);
            item->setTextAlignment(Qt::AlignLeft);
            item->setSizeHint(QSize(100 , 100));
        }else{
            QListWidgetItem *item = new QListWidgetItem(QString("ali mohseni : tnx and u?") , ui->chatsList);
            item->setTextAlignment(Qt::AlignRight);
            item->setSizeHint(QSize(100 , 100));
        }

    }





}

ChatPage::~ChatPage()
{

    delete ui;
}

void ChatPage::getUsersList()
{

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

                        for(auto it = jsonObj.begin() ; it != jsonObj.end(); it++){
                            if(it.value().isObject()){
                                QString username = it.value().toObject().value("src").toString();
                                ui->messagesList->addItem(username);
                            }

                        }
                    }
                    else{
                        qDebug() << "error";
                    }

                }
                reply->deleteLater();
            });


}

void ChatPage::getGroupList()
{
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

                        for(auto it = jsonObj.begin() ; it != jsonObj.end(); it++){
                            if(it.value().isObject()){
                                QString groupName = it.value().toObject().value("group_name").toString();
                                ui->messagesList->addItem(groupName);
                            }

                        }
                    }
                    else{
                        qDebug() << "error";
                    }

                }
                reply->deleteLater();
            });


}

void ChatPage::getChannelList()
{
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

                        for(auto it = jsonObj.begin() ; it != jsonObj.end(); it++){
                            if(it.value().isObject()){
                                QString channelName = it.value().toObject().value("channel_name").toString();
                                ui->messagesList->addItem(channelName);
                            }

                        }
                    }
                    else{
                        qDebug() << "error";
                    }

                }
                reply->deleteLater();
            });

}
