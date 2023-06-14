#ifndef QUERIES_H
#define QUERIES_H
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QString>
#include <QVector>
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
struct TableChats{
    QString src{""};
    QString dst{""};
    QString body{""};
    QString date{""};

};
//table info functions
void createTblInfo();
TableInfo selectTblinfo();
void insertTblInfo(QString token , QString username, QString password,QString title);
void emptyTblInfo();
//table chats_list functions
void createTblChatsList();
void insertTblChatsList(QString username , QString title, int type);
QVector<TableChatsList> selectTblChatsList(int type);
void emptyTblChatsList();
void deleteTblChatsList(int type);
//table chats functions
void createTblChats();
void emptyTblChats();
void insertTblChats(QString src, QString dst, QString body,QString date);
QVector<TableChats> selectTblChats(QString src , QString dst);
}
#endif // QUERIES_H
