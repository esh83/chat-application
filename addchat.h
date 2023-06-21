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

private:
    Ui::AddChat *ui;
};

#endif // ADDCHAT_H
