#include "setupmain.h"
#include "ui_setupmain.h"

SetupMain::SetupMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupMain)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

SetupMain::~SetupMain()
{
    delete ui;
}

void SetupMain::on_btn_start_signup_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void SetupMain::on_btn_start_login_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}



void SetupMain::on_btn_signup_cancel_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void SetupMain::on_btn_login_cancel_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}

