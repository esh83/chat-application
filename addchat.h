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
    explicit AddChat(QString token,QWidget *parent = nullptr);
    ~AddChat();

private slots:
    void on_btn_create_clicked();
    void on_btn_cancel_clicked();

private:
    Ui::AddChat *ui;
    QString m_token;
};

#endif // ADDCHAT_H
