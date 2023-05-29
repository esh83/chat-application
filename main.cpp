#include "setupmain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SetupMain w;
    w.show();
    return a.exec();
}
