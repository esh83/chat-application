#ifndef QUERIES_H
#define QUERIES_H
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QString>
namespace DB{
struct TableInfo {
    QString token{""};
    QString username{""};
    QString password{""};
    QString title{""};

};
struct TableChatsList{
    QString username{""};
    QString title{""};
    int type;
};
//table info functions
void createTblInfo();
TableInfo selectTblinfo();
void insertTblInfo(QString token , QString username, QString password,QString title);
void emptyTblInfo();
//table chats list functions
void createTblChatsList();
void insertTblChatsList(QString username , QString title, int type);
TableChatsList selectTblChatsList(int id,int type);
void emptyTblChatsList();
}
#endif // QUERIES_H
