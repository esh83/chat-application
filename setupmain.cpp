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


SetupMain::SetupMain(QWidget *parent) :
    QDialog(parent),
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


void SetupMain::on_btn_login_login_clicked()
{
    if(ui->input_login_username->text().isEmpty() || ui->input_login_password->text().isEmpty()){
        QMessageBox::warning(this, tr("Warning"), tr("Please enter username and password!"));
        return;
    }

    QString message;
    QString code;
    QString token;



    QString username= ui->input_login_username->text();
    QString password= ui->input_login_password->text();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url(QString(API_ADRESS)+"/login?username="+username+"&password="+password);
    QNetworkRequest request(url);



    QNetworkReply* reply = manager->get(request);


    connect(reply, &QNetworkReply::finished, [=]() mutable
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug()<<"Login Error: " << reply->errorString();
        }
        else
        {
            QByteArray response = reply->readAll();
            qDebug()<<response;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            QJsonObject jsonObj = jsonDoc.object();
            message = jsonObj.value("message").toString();
            code = jsonObj.value("code").toString();
            token = jsonObj.value("token").toString();

            if (code == "200")
            {

                ChatPage* chat = new ChatPage( token);
                connect(chat , SIGNAL(finished(int)) , chat , SLOT(deleteLater()));
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

        }
        reply->deleteLater();
    });


}


void SetupMain::on_btn_signup_signup_clicked()
{
    if(ui->input_signup_username->text().isEmpty() || ui->input_signup_password->text().isEmpty()){
        QMessageBox::warning(this, tr("Warning"), tr("Please enter username and password!"));
        return;
    }


    QString message;
    QString code;


    QString  username =ui->input_signup_username->text();
    QString  password =ui->input_signup_password->text();
    QString  firstname =ui->input_firstname->text();
    QString  lastname =ui->input_lastname->text();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QUrl url(QString(API_ADRESS)+"/signup?username="+username+"&password="+password+"&firstname="+firstname+"&lastname"+lastname);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    QObject::connect(reply, &QNetworkReply::finished, [=]()mutable
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug()<<"Signup Error: " << reply->errorString();
        }
        else
        {
            QByteArray response = reply->readAll();
                qDebug()<<response;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            QJsonObject jsonObj = jsonDoc.object();
            code = jsonObj.value("code").toString();
            message = jsonObj.value("message").toString();
            if (code == "200")
            {
                QMessageBox::information(this ,"success" ,"signed up successfully try to login");
                ui->stackedWidget->setCurrentIndex(1);

            }
            else if(code == "204")
            {
                QMessageBox::warning(this ,"error" ,"username already exist try again");

            }else{
                QMessageBox::warning(this ,"error" ,"sign up failed");

            }


        }
        reply->deleteLater();
    });


}




