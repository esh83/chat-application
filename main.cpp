#include "setup.h"

#include <QApplication>
#include <QtWidgets>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setup w;
    w.show();
    return a.exec();
}
