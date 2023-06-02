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
#include "config.h"
namespace Ui {
class ChatPage;
}

class ChatPage : public QDialog
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr , QString token = "");
    ~ChatPage();

private:
    void getUsersList();
    Ui::ChatPage *ui;
    QString m_token;
};

#endif // CHATPAGE_H
