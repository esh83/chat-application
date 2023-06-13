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

#include "updateThread.h"
namespace Ui {
class ChatPage;
}

class ChatPage : public QDialog
{
    Q_OBJECT

public:
    explicit ChatPage(QString password="" ,QString username="", QString token = "" ,QWidget *parent = nullptr);
    ~ChatPage();

private slots:
    void on_messagesList_chat_itemClicked(QListWidgetItem *item);

    void on_messagesList_group_itemClicked(QListWidgetItem *item);

    void on_messagesList_channel_itemClicked(QListWidgetItem *item);

    void getUsersList();

    void getGroupList();

    void getChannelList();

    void updateCurrentChatMessages();

    void on_btn_logout_clicked();

private:

    Ui::ChatPage *ui;
    QString m_token;
    QString m_username;
    QString m_password;
    UpdateThread* m_updateThread;


};

#endif // CHATPAGE_H
