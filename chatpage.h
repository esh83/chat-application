#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QDialog>

#include <QWidget>
namespace Ui {
class ChatPage;
}

class ChatPage : public QDialog
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();

private:
    Ui::ChatPage *ui;
};

#endif // CHATPAGE_H
