#ifndef ITEMCHAT_H
#define ITEMCHAT_H

#include <QWidget>

namespace Ui {
class ItemChat;
}

class ItemChat : public QWidget
{
    Q_OBJECT

public:
    explicit ItemChat(QWidget *parent = nullptr);
    ~ItemChat();

private:
    Ui::ItemChat *ui;
};

#endif // ITEMCHAT_H
