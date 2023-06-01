#include "itemchat.h"
#include "ui_itemchat.h"

ItemChat::ItemChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemChat)
{
    ui->setupUi(this);

}

ItemChat::~ItemChat()
{
    delete ui;
}
