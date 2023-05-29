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


void SetupMain::on_btn_login_login_clicked()
{
     if(ui->input_login_username->text().isEmpty() || ui->input_login_password->text().isEmpty()){
         QMessageBox::warning(this, tr("Warning"), tr("Please enter username and password!"));
         return;
     }

     QString username= ui->input_login_username->text();
     QString password= ui->input_login_password->text();

     QNetworkAccessManager *manager = new QNetworkAccessManager(this);
     QUrl url(QString(API_ADRESS)+"/login?username="+username+"&password="+password);
     QNetworkRequest request(url);

     //*


     QNetworkReply* reply = manager->get(request);

     // connect signals and slots to handle the response
     connect(reply, &QNetworkReply::finished, [=]() mutable {
         if (reply->error() != QNetworkReply::NoError) {
             qDebug()<<"Login Error: " << reply->errorString();
         } else {
             QByteArray response = reply->readAll();
             qDebug()<<response;

              qDebug()<<"Logged in successfully with token: ";
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


     QString  username =ui->input_signup_username->text();
     QString  password =ui->input_signup_password->text();
     QString  firstname =ui->input_firstname->text();
     QString  lastname =ui->input_lastname->text();

     QNetworkAccessManager *manager = new QNetworkAccessManager(this);

     QUrl url(QString(API_ADRESS)+"/sighnup?username="+username+"password="+password+"firstname="+firstname+"lastname"+lastname);
     QNetworkRequest request(url);



     QNetworkReply* reply = manager->get(request);

     QObject::connect(reply, &QNetworkReply::finished, [=]() mutable {
         if (reply->error() != QNetworkReply::NoError) {
             qDebug()<<"Signup Error: " << reply->errorString();
         } else {
             QByteArray response = reply->readAll();
             QString message = QString(response);

             if (message == "up signed successfully") {
                 qDebug()<<"Signed up successfully.";
             } else {
                 qDebug()<<"Signup Error: " + message;
             }
         }
         reply->deleteLater();
     });
}

