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
#include "requesthandler.h"
#include "workerlist.h"
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
    void getChat(QString item , QString endpoint , int type);
    void on_messagesList_chat_itemClicked(QListWidgetItem* item);
    void getUserChat(QString item);


    void on_messagesList_group_itemClicked(QListWidgetItem *item);
    void getGroupChat(QString item);


    void on_messagesList_channel_itemClicked(QListWidgetItem *item);
    void getChannelChat(QString item);


    void handleUserListResult(QVector<QString> result);
     void handleChannelListResult(QVector<QString> result);
     void handleGroupListResult(QVector<QString> result);
     void handleFailedListResult();
    void getUsersList();

    void getGroupList();

    void getChannelList();

    void updateCurrentChatMessages();

    void on_btn_logout_clicked();

    void on_tabWidget_currentChanged(int index);
    void on_btn_sendMessage_clicked();

    void on_btn_scrollBottom_clicked();

    void sendChatMessage(QString dst, QString body, QString date);

    void getList(int chatType, QListWidget* listWidget, const QString& endpoint);

    void on_btn_new_channel_clicked();


    void on_btn_new_group_clicked();

    void on_btn_new_chat_clicked();

private:

private:
    Ui::ChatPage *ui;
    QString m_token;
    QString m_username;
    QString m_password;
    int m_tabIndex;
    int currentTab{0};
    int m_selectedChatIndex{-1};
    QString m_currentChatName;
    WorkerList *m_workerlist;
    QThread *m_workerThread;


};

#endif // CHATPAGE_H
