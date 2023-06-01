#include "chatpage.h"
#include "ui_chatpage.h"
#include "itemchat.h"
#include <QSize>
#include <QBrush>
#include <QColor>
ChatPage::ChatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPage)
{
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
    for(int i =0 ; i < 10; i++){
        QListWidgetItem *item = new QListWidgetItem(QString("Digital Marketing GP %1\nvahid azari : hello guys whats up dudes how you doing").arg(QString::number(i)) , ui->messagesList);
        item->setTextAlignment(Qt::AlignLeft);

    }
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
