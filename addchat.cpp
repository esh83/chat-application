#include "addchat.h"
#include "ui_addchat.h"

AddChat::AddChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChat)
{
    ui->setupUi(this);
}

AddChat::~AddChat()
{
    delete ui;
}
