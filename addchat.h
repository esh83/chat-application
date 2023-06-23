#ifndef ADDCHAT_H
#define ADDCHAT_H

#include <QDialog>

namespace Ui {
class AddChat;
}

class AddChat : public QDialog
{
    Q_OBJECT

public:
    explicit AddChat(QString token,int chatType, QString endpoint ,QWidget *parent = nullptr);
    ~AddChat();

private slots:
    void on_btn_create_clicked();
    void on_btn_cancel_clicked();

    void on_input_chat_name_textEdited(const QString &arg1);

private:
    Ui::AddChat *ui;
    QString m_token;
    int m_chatType;
    QString m_endpoint;
};

#endif // ADDCHAT_H
