#include "addchat.h"
#include "ui_addchat.h"
#include "chatpage.h"

AddChat::AddChat(QString token,int chatType, QString endpoint ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChat) , m_token{token} , m_chatType{chatType} , m_endpoint{endpoint}
{
    ui->setupUi(this);
    this->setWindowTitle("Add Chat");
    if(m_chatType==1)
    {
        ui->dialog_title->setText("creating chat");
        ui->lbl_title->setText("first message to user");
        ui->lbl_name->setText("username");
    }
    else if(m_chatType==2)
    {
        ui->dialog_title->setText("creating channel");
        ui->lbl_name->setText("channel name");
        ui->lbl_title->setText("channel title");
        ui->lbl_dst->hide();

    }
    else
    {
        ui->dialog_title->setText("creating group");
        ui->lbl_name->setText("group name");
        ui->lbl_title->setText("group title");
        ui->lbl_dst->hide();

    }
}

AddChat::~AddChat()
{
    delete ui;
}

void AddChat::on_btn_create_clicked()
{

    if(ui->input_chat_name->text().isEmpty()){
        QMessageBox::warning(this, "Warning", "Please enter name!");
        ui->btn_create->setText("Create");
        ui->btn_create->setDisabled(false);
        return;
    }

    if(m_chatType==1 && ui->input_chat_title->text().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please write first message!");
        ui->btn_create->setText("Create");
        ui->btn_create->setDisabled(false);
        return;
    }

    ui->btn_create->setDisabled(true);
    ui->btn_create->setText("Creating ...");
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
            this->close();

        }
        else
        {
            QMessageBox::warning(this ,"error" ,message);

        }
        ui->btn_create->setText("Create");
        ui->btn_create->setDisabled(false);
    });


    if(m_chatType==2 || m_chatType==3)
    {
        QString title = (ui->input_chat_title->text() != "") ? ("group_title=" + ui->input_chat_title->text()) : "";
        QString type_name = (m_chatType == 3) ? ("&group_name=") : "&channel_name=";
        req_hadler->fetchData(QString(API_ADRESS)+m_endpoint+"?token="+ m_token +type_name+ name_chat+title);
    }
    else
    {
        QString new_chat = "&dst="+ui->input_chat_name->text()+"&body="+ui->input_chat_title->text();
        req_hadler->fetchData(QString(API_ADRESS)+m_endpoint+"?token="+m_token+new_chat);
    }
}


void AddChat::on_btn_cancel_clicked()
{
    this->close();
}



void AddChat::on_input_chat_name_textEdited(const QString &arg1)
{
    if(m_chatType==1)
        ui->lbl_dst->setText(arg1);
}

