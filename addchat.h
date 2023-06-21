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
    explicit AddChat(QWidget *parent = nullptr);
    ~AddChat();

private slots:
    void on_btn_creat_clicked();

    void on_btn_creat_cancel_clicked();

private:
    Ui::AddChat *ui;
};

#endif // ADDCHAT_H
