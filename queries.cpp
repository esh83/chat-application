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
        qDebug( "Inserted!" );
    }
}

void DB::emptyTblInfo()
{
    QSqlQuery qry;
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
    QSqlQuery qry;
    qry.prepare( "INSERT INTO tblchatslist (username , title,type) VALUES (:uname,:tit,:typ)" );
    qry.bindValue(":uname" , username);
    qry.bindValue(":tit" , title);
    qry.bindValue(":typ" , type);


    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else{
        qDebug( "Inserted!" );
    }
}

DB::TableChatsList DB::selectTblChatsList(int id , int type)
{
    QSqlQuery qry;
    qry.prepare( "SELECT username,title,type FROM tblchatslist WHERE id=:id AND type=:typ" );
    qry.bindValue(":id" , id);
     qry.bindValue(":typ" , type);
    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else
    {

        if(qry.next()){
           TableChatsList tbl;

           tbl.title = qry.value(qry.record().indexOf("title")).toString();
            tbl.username = qry.value(qry.record().indexOf("username")).toString();
             tbl.type = qry.value(qry.record().indexOf("type")).toInt();
            return tbl;
        }else{
           throw qry.lastError().text();
        }

    }
}

void DB::emptyTblChatsList()
{
    QSqlQuery qry;
    qry.prepare("DELETE FROM tblchatslist");
    if( !qry.exec() ){
        throw qry.lastError().text();
    }
}

