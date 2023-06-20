#include "setupmain.h"
#include "ui_setupmain.h"
#include <QtCore>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <config.h>
#include <QMessageBox>
#include "chatpage.h"
#include "queries.h"
#include "requesthandler.h"


SetupMain::SetupMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupMain)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    try{
        DB::createTblInfo();
    }catch(QString &err){
        qDebug()<<err;
    }


    try{
        DB::TableInfo info;
        info = DB::selectTblinfo();
        if(info.token!=""){
            ChatPage* chat = new ChatPage( info.password , info.username, info.token);
            connect(chat , SIGNAL(finished(int)) , chat , SLOT(deleteLater()));
            connect(chat , SIGNAL(accepted()) , this , SLOT(show()));
            chat->show();
            QTimer::singleShot(100, this, SLOT(hide()));

        }
    }catch(QString &err){
        qDebug()<<err;
    }
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


void SetupMain::on_btn_login_login_clicked()
{
    if(ui->input_login_username->text().isEmpty() || ui->input_login_password->text().isEmpty()){
        QMessageBox::warning(this, "Warning", "Please enter username and password!");
        return;
    }

    ui->btn_login_login->setDisabled(true);
    ui->btn_login_login->setText("Loading ...");
    QString username= ui->input_login_username->text();
    QString password= ui->input_login_password->text();
    RequestHandler *req_hadler = new RequestHandler(this);
    connect(req_hadler,&RequestHandler::errorOccured,[=](QString err){
      QMessageBox::warning(this ,"error" ,"something went wrong");
      qDebug()<<err;
      ui->btn_login_login->setText("Login");
      ui->btn_login_login->setDisabled(false);
    });
    connect(req_hadler,&RequestHandler::dataReady,[=](QJsonObject jsonObj){
       QString message = jsonObj.value("message").toString();
       QString code = jsonObj.value("code").toString();
       QString token = jsonObj.value("token").toString();

        if (code == "200")
        {
            DB::insertTblInfo(token,username,password ,"John Doe");
            ChatPage* chat = new ChatPage(password ,username , token);
            connect(chat , SIGNAL(finished(int)) , chat , SLOT(deleteLater()));
            connect(chat , SIGNAL(accepted()) , this , SLOT(show()));
            chat->show();
            this->hide();

        }
        else if(code == "404")
        {
            QMessageBox::warning(this ,"error" ,"user not found");

        }else if(code == "401"){
            QMessageBox::warning(this ,"error" ,message);

        }else{
            QMessageBox::warning(this ,"error" ,"login failed");
        }
        ui->btn_login_login->setText("Login");
        ui->btn_login_login->setDisabled(false);
    });
    req_hadler->fetchData(QString(API_ADRESS)+"/login?username="+username+"&password="+password);
}


void SetupMain::on_btn_signup_signup_clicked()
{
    if(ui->input_signup_username->text().isEmpty() || ui->input_signup_password->text().isEmpty()){
        QMessageBox::warning(this, tr("Warning"), tr("Please enter username and password!"));
        return;
    }
    ui->btn_signup_signup->setDisabled(true);
    ui->btn_signup_signup->setText("Loading ...");
    QString  username =ui->input_signup_username->text();
    QString  password =ui->input_signup_password->text();
    QString  firstname =ui->input_firstname->text();
    QString  lastname =ui->input_lastname->text();

    RequestHandler *req_hadler = new RequestHandler(this);
    connect(req_hadler,&RequestHandler::errorOccured,[=](QString err){
        QMessageBox::warning(this ,"error" ,"something went wrong");
        qDebug()<<err;
        ui->btn_signup_signup->setText("SignUp");
        ui->btn_signup_signup->setDisabled(false);
    });
    connect(req_hadler,&RequestHandler::dataReady,[=](QJsonObject jsonObj){
       QString code = jsonObj.value("code").toString();
       //QString message = jsonObj.value("message").toString();
        if (code == "200")
        {
            QMessageBox::information(this ,"success" ,"signed up successfully try to login");
            ui->stackedWidget->setCurrentIndex(1);
            ui->input_firstname->clear();
            ui->input_lastname->clear();
            ui->input_login_password->clear();
            ui->input_login_username->clear();
            ui->input_signup_password->clear();
            ui->input_signup_username->clear();

        }
        else if(code == "204")
        {
            QMessageBox::warning(this ,"error" ,"username already exist try again");

        }else{
            QMessageBox::warning(this ,"error" ,"sign up failed");

        }
        ui->btn_signup_signup->setText("SignUp");
        ui->btn_signup_signup->setDisabled(false);
    });
    req_hadler->fetchData(QString(API_ADRESS)+"/signup?username="+username+"&password="+password+"&firstname="+firstname+"&lastname"+lastname);


}




