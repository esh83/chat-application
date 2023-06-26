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
#include "workerchat.h"
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

    void on_messagesList_chat_itemClicked(QListWidgetItem* item);
    void on_messagesList_group_itemClicked(QListWidgetItem *item);
    void on_messagesList_channel_itemClicked(QListWidgetItem *item);

    void handleUserListResult(QVector<QString> result);
    void handleChannelListResult(QVector<QString> result);
    void handleGroupListResult(QVector<QString> result);
    void handleFailedListResult();
    void handleSuccessListResult();
    void handleChatResult(QVector<chatMsg> result);
    void handleChatSended();
    void handleSendingFailed();

    void on_btn_logout_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_btn_sendMessage_clicked();
    void on_btn_scrollBottom_clicked();

    void on_btn_new_channel_clicked();
    void on_btn_new_group_clicked();
    void on_btn_new_chat_clicked();

    void on_btn_join_channel_clicked();

    void on_btn_join_group_clicked();

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
    WorkerChat *m_workerchat;
    QThread *m_workerThread;
    bool m_shoud_scroll{false};


};

#endif // CHATPAGE_H
