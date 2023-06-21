#include "addchat.h"
#include "ui_addchat.h"
#include "chatpage.h"

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

void AddChat::on_btn_creat_clicked()
{
    ui->btn_creat->setDisabled(true);
    ui->btn_creat->setText("Creating ...");
    if(ui->name_chat->text().isEmpty()){
        QMessageBox::warning(this, "Warning", "Please enter name!");
        return;
    }


}


void AddChat::on_btn_creat_cancel_clicked()
{
     ui->btn_creat_cancel->setDisabled(true);
    this->close();


}

