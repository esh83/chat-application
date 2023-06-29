#include "queries.h"

void DB::createTblInfo()
{
    QSqlQuery qry;
    qry.prepare("CREATE TABLE IF NOT EXISTS tblinfo (id INTEGER UNIQUE PRIMARY KEY, token TEXT , username TEXT ,password TEXT , title TEXT)");
    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else
        qDebug() << "Table Info created!";
}

DB::TableInfo DB::selectTblinfo()
{
    QSqlQuery qry;
    qry.prepare( "SELECT token, username ,password , title FROM tblinfo" );
    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else
    {

        if(qry.next()){
            TableInfo tblinfo;

            tblinfo.token = qry.value(qry.record().indexOf("token")).toString();
            tblinfo.username = qry.value(qry.record().indexOf("username")).toString();
            tblinfo.password = qry.value(qry.record().indexOf("password")).toString();
            tblinfo.title = qry.value(qry.record().indexOf("title")).toString();
            return tblinfo;
        }else{
            throw qry.lastError().text();
        }

    }
}

void DB::insertTblInfo(QString token , QString username, QString password,QString title)
{
    QSqlQuery qry;
    qry.prepare("DELETE FROM tblinfo");
    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    qry.prepare( "INSERT INTO tblinfo (token,username,password,title) VALUES (:tkn,:uname,:pass,:tit)" );
    qry.bindValue(":tkn" , token);
    qry.bindValue(":uname" , username);
    qry.bindValue(":pass" , password);
    qry.bindValue(":tit" , title);

    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else{
        //qDebug( "Inserted!" );
    }
}

void DB::emptyTblInfo()
{
    QSqlQuery qry(QSqlDatabase::database("worker_db"));
    qry.prepare( "UPDATE tblinfo SET token=\"\",username=\"\",password=\"\",title=\"\"" );

    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else{
        qDebug( "emptyed!" );

    }

}

void DB::createTblChatsList()
{
    QSqlQuery qry;
    qry.prepare("CREATE TABLE IF NOT EXISTS tblchatslist (id INTEGER UNIQUE PRIMARY KEY, username TEXT , title TEXT ,type INTEGER)");
    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else
        qDebug() << "Table Chats List created!";
}

void DB::insertTblChatsList(QString username, QString title, int type)
{
    QSqlQuery qry(QSqlDatabase::database("worker_db"));
    qry.prepare( "INSERT INTO tblchatslist (username , title,type) VALUES (:uname,:tit,:typ)" );
    qry.bindValue(":uname" , username);
    qry.bindValue(":tit" , title);
    qry.bindValue(":typ" , type);


    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else{
        //qDebug( "Inserted!" );
    }
}

QVector<DB::TableChatsList> DB::selectTblChatsList(int type)
{
    QSqlQuery qry(QSqlDatabase::database("worker_db"));
    QVector<DB::TableChatsList> list;
    qry.prepare( "SELECT username,title,type FROM tblchatslist WHERE type=:typ" );
    qry.bindValue(":typ" , type);
    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else
    {

        while(qry.next()){
            TableChatsList tbl;

            tbl.title = qry.value(qry.record().indexOf("title")).toString();
            tbl.username = qry.value(qry.record().indexOf("username")).toString();
            tbl.type = qry.value(qry.record().indexOf("type")).toInt();
            list.append(tbl);
        }

    }
    return list;
}

void DB::emptyTblChatsList()
{
    QSqlQuery qry(QSqlDatabase::database("worker_db"));
    qry.prepare("DELETE FROM tblchatslist");
    if( !qry.exec() ){
        throw qry.lastError().text();
    }else{
        qDebug() << "emptyed";
    }
}


void DB::deleteTblChatsList(int type)
{
    QSqlQuery qry(QSqlDatabase::database("worker_db"));
    qry.prepare("DELETE FROM tblchatslist WHERE type=:typ");
    qry.bindValue(":typ",type);
    if( !qry.exec() ){
        throw qry.lastError().text();
    }
}

void DB::createTblChats()
{
    QSqlQuery qry;
    qry.prepare("CREATE TABLE IF NOT EXISTS tblchats (id INTEGER UNIQUE PRIMARY KEY, src TEXT, dst TEXT, body TEXT, date TEXT)");
    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else
        qDebug() << "Table Chats created!";
}
void DB::emptyTblChats()
{
    QSqlQuery qry(QSqlDatabase::database("worker_db"));
    qry.prepare("DELETE FROM tblchats");
    if( !qry.exec() ){
        throw qry.lastError().text();
    }else{
        qDebug() << "emptyed";
    }
}
void DB::insertTblChats(QString src, QString dst, QString body,QString date)
{
    QSqlQuery qry(QSqlDatabase::database("worker_db"));
    qry.prepare( "SELECT * FROM tblchats WHERE src=:src AND dst=:dst AND body=:body AND date=:date" );
    qry.bindValue(":src" , src);
    qry.bindValue(":dst" , dst);
    qry.bindValue(":body" , body);
    qry.bindValue(":date" , date);
    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else{
        if(qry.next()){
            return;
        }
    }
    qry.prepare( "INSERT INTO tblchats (src,dst,body,date) VALUES (:src,:dst,:body,:date)" );
    qry.bindValue(":src" , src);
    qry.bindValue(":dst" , dst);
    qry.bindValue(":body" , body);
    qry.bindValue(":date" , date);


    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else{
        //qDebug( "Inserted!" );
    }
}

QVector<DB::TableChats> DB::selectTblChats(QString src, QString dst)
{
    QSqlQuery qry(QSqlDatabase::database("worker_db"));
    QVector<DB::TableChats> list;
    if(src == "*"){
        qry.prepare( "SELECT src,dst,body,date FROM tblchats WHERE dst=:dst" );
        qry.bindValue(":dst" , dst);
    }
    else{
        qry.prepare( "SELECT src,dst,body,date FROM tblchats WHERE (src=:src AND dst=:dst) OR (src=:dst AND dst=:src)" );
        qry.bindValue(":src" , src);
        qry.bindValue(":dst" , dst);
    }

    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else
    {

        while(qry.next()){
            TableChats tbl;

            tbl.src = qry.value(qry.record().indexOf("src")).toString();
            tbl.dst = qry.value(qry.record().indexOf("dst")).toString();
            tbl.body = qry.value(qry.record().indexOf("body")).toString();
            tbl.date = qry.value(qry.record().indexOf("date")).toString();
            list.append(tbl);
        }

    }
    return list;
}
