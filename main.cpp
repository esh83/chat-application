#include "chatpage.h"
#include "setupmain.h"
#include <QApplication>
#include <QtWidgets>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatPage w;
    w.show();
    return a.exec();
}
