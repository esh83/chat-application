#include "addchat.h"
#include "ui_addchat.h"
#include "chatpage.h"

AddChat::AddChat(QString token,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChat) , m_token{token}
{
    ui->setupUi(this);
}

AddChat::~AddChat()
{
    delete ui;
}

void AddChat::on_btn_create_clicked()
{
    ui->btn_create->setDisabled(true);
    ui->btn_create->setText("Creating ...");
    if(ui->input_chat_name->text().isEmpty()){
        QMessageBox::warning(this, "Warning", "Please enter name!");
        return;
    }
    ui->btn_create->setDisabled(true);
    ui->btn_create->setText("Loading ...");
    QString name_chat= ui->input_chat_name->text();
    RequestHandler *req_hadler = new RequestHandler(this);
    connect(req_hadler,&RequestHandler::errorOccured,[=](QString err){
        QMessageBox::warning(this ,"error" ,"something went wrong");
        qDebug()<<err;
        ui->btn_create->setText("Create");
        ui->btn_create->setDisabled(false);
    });
    connect(req_hadler,&RequestHandler::dataReady,[=](QJsonObject jsonObj){

        QString code = jsonObj.value("code").toString();
        QString message = jsonObj.value("message").toString();

        if (code == "200")
        {
            QMessageBox::information(this, "success", message);

        }
        else
        {
            QMessageBox::warning(this ,"error" ,message);

        }
        ui->btn_create->setText("Create");
        ui->btn_create->setDisabled(false);
    });
    req_hadler->fetchData(QString(API_ADRESS)+"/createchannel?token="+ m_token +"&channel_name="+ name_chat);
}


void AddChat::on_btn_cancel_clicked()
{
    ui->btn_cancel->setDisabled(true);
    this->close();
}

