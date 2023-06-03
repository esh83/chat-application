#ifndef QUERIES_H
#define QUERIES_H
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QString>
struct TableInfo {
    QString token{""};
    QString username{""};
    QString password{""};
    QString title{""};

};

void createTblInfo();
TableInfo selectTblinfo();
void insertTblInfo(QString token , QString username, QString password,QString title);
void emptyTblInfo();


#endif // QUERIES_H
