#include "workerother.h"
#include <QCoreApplication>
WorkerOther::WorkerOther(QString username, QString password,QObject *parent)
    : QObject{parent},m_username{username},m_password{password}
{

}

void WorkerOther::logout()
{
    RequestHandler *req_handler = new RequestHandler(this);
    connect(req_handler,&RequestHandler::errorOccured,[=](QString err){
        qDebug()<<err;
        emit failed(err);
    });
    connect(req_handler,&RequestHandler::dataReady,[=](QJsonObject jsonObj ){
        QString message = jsonObj.value("message").toString();
        QString code = jsonObj.value("code").toString();
        if (code == "200")
        {
            //RESET LOCAL DATABASE
            DB::emptyTblInfo();
            DB::emptyTblChatsList();
            DB::emptyTblChats();
            emit done();
            emit success();

        }
        else{
            emit done();
            emit failed(message);
        }

    });
    req_handler->fetchData(QString(API_ADRESS)+"/logout?username="+m_username+"&password="+m_password);
}

void WorkerOther::openDB()
{
    QSqlDatabase::removeDatabase("worker_others_db");
    QString path = QCoreApplication::applicationDirPath();
    QString dbPath = path + "/data.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",  "worker_others_db");
    db.setDatabaseName(dbPath);
    if (!db.open()) {
        qDebug() << db.lastError().text();
    } else {
        db.exec("PRAGMA journal_mode = WAL;");
        db.exec("PRAGMA cache_size = -8192;");
        qDebug() << "Database opened successfully";
    }
}

