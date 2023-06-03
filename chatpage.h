#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QDialog>
#include <QString>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QListWidgetItem>
#include "config.h"
namespace Ui {
class ChatPage;
}

class ChatPage : public QDialog
{
    Q_OBJECT

public:
    explicit ChatPage(QString username, QString token = "" ,QWidget *parent = nullptr);
    ~ChatPage();

private slots:
    void on_messagesList_chat_itemClicked(QListWidgetItem *item);

    void on_messagesList_group_itemClicked(QListWidgetItem *item);

private:
    void getUsersList();
    void getGroupList();
    void getChannelList();
    Ui::ChatPage *ui;
    QString m_token;
    QString m_username;

};

#endif // CHATPAGE_H
