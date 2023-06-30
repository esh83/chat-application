#include "addchat.h"
#include "ui_addchat.h"
#include "chatpage.h"

AddChat::AddChat(QString token,int chatType, QString endpoint ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChat) , m_token{token} , m_chatType{chatType} , m_endpoint{endpoint}
{
    ui->setupUi(this);
    if(m_endpoint!="/joinchannel" && m_endpoint!="/joingroup")
    {
        ui->btn_create->setText("Create");
        this->setWindowTitle("Add Chat");
    }
    else
    {
        ui->btn_create->setText("Join");
        this->setWindowTitle("Join Chat");
    }

    if(m_chatType==1)
    {
        ui->dialog_title->setText("Create chat");
        ui->lbl_title->setText("First message to user");
        ui->lbl_name->setText("Username");
    }
    else if(m_chatType==2)
    {
        if(m_endpoint=="/createchannel")
        {
            ui->dialog_title->setText("Create channel");
            ui->lbl_name->setText("Channel name");
            ui->lbl_title->setText("Channel title");
            ui->lbl_dst->hide();
        }
        else
        {
            ui->dialog_title->setText("Join channel");
            ui->lbl_name->setText("Channel name");
            ui->input_chat_title->hide();
            ui->lbl_title->hide();
            ui->lbl_dst->hide();
        }

    }
    else
    {
        if(m_endpoint=="/creategroup")
        {
            ui->dialog_title->setText("Create group");
            ui->lbl_name->setText("Group name");
            ui->lbl_title->setText("Group title");
            ui->lbl_dst->hide();
        }
        else
        {
            ui->dialog_title->setText("Join Group");
            ui->lbl_name->setText("Group name");
            ui->input_chat_title->hide();
            ui->lbl_title->hide();
            ui->lbl_dst->hide();
        }


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
        if(m_endpoint!="/joinchannel" && m_endpoint!="/joingroup")
        ui->btn_create->setText("Create");
        else
        ui->btn_create->setText("Join");
        ui->btn_create->setDisabled(false);
        return;
    }

    if(m_chatType==1 && ui->input_chat_title->text().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please write first message!");
        if(m_endpoint!="/joinchannel" && m_endpoint!="/joingroup")
        ui->btn_create->setText("Create");
        else
        ui->btn_create->setText("Join");
        ui->btn_create->setDisabled(false);
        return;
    }

    ui->btn_create->setDisabled(true);
    if(m_endpoint!="/joinchannel" && m_endpoint!="/joingroup")
    ui->btn_create->setText("Creating ...");
    else
    ui->btn_create->setText("Joining ...");

    QString name_chat= ui->input_chat_name->text();
    RequestHandler *req_hadler = new RequestHandler(this);
    connect(req_hadler,&RequestHandler::errorOccured,[=](QString err){
        QMessageBox::warning(this ,"error" ,"something went wrong");
        qDebug()<<err;
        if(m_endpoint!="/joinchannel" && m_endpoint!="/joingroup")
            ui->btn_create->setText("Create");
        else
            ui->btn_create->setText("Join");
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
        if(m_endpoint!="/joinchannel" && m_endpoint!="/joingroup")
            ui->btn_create->setText("Create");
        else
            ui->btn_create->setText("Join");
        ui->btn_create->setDisabled(false);
    });

    if(m_endpoint!="/joinchannel" && m_endpoint!="/joingroup")
    {
        if(m_chatType==2 || m_chatType==3)
        {

            QString title = (m_chatType == 3) ? ("&group_title=" + ui->input_chat_title->text()) : ("&channel_title=" + ui->input_chat_title->text());
            QString type_name = (m_chatType == 3) ? ("&group_name=") : "&channel_name=";
            req_hadler->fetchData(QString(API_ADRESS)+m_endpoint+"?token="+ m_token +type_name+ name_chat+title);
        }
        else
        {
            QString new_chat = "&dst="+ui->input_chat_name->text()+"&body="+ui->input_chat_title->text();
            req_hadler->fetchData(QString(API_ADRESS)+m_endpoint+"?token="+m_token+new_chat);
        }
    }
    else
    {
        if(m_endpoint=="/joinchannel")
            req_hadler->fetchData(QString(API_ADRESS)+m_endpoint+"?token="+ m_token +"&channel_name="+ name_chat);
        else
            req_hadler->fetchData(QString(API_ADRESS)+m_endpoint+"?token="+ m_token +"&group_name="+ name_chat);


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

