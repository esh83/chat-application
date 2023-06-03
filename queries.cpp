#include "queries.h"

void createTblInfo()
{
    QSqlQuery qry;
    qry.prepare("create table if not exists tblinfo (id integer unique primary key, token text , username text ,password text , title text)");
    if( !qry.exec() ){
        throw qry.lastError().text();
    }
    else
        qDebug() << "Table created!";
}

TableInfo selectTblinfo()
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

void insertTblInfo(QString token , QString username, QString password,QString title)
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

void emptyTblInfo()
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
