#include "setup.h"
#include "ui_setup.h"
#include "logindialog.h"
#include "signupdialog.h"

setup::setup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setup)
{
    ui->setupUi(this);
}

setup::~setup()
{
    delete ui;
}


void setup::on_btn_login_clicked()
{
    LoginDialog * logindialog = new LoginDialog();
    this->hide();
    logindialog->exec();
    delete logindialog;
}


void setup::on_btn_signup_clicked()
{
    SignUpDialog * signupdialog = new SignUpDialog();
    this->hide();
    signupdialog->exec();
    delete signupdialog;
}

