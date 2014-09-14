#include "mainclass.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainClass w;
    w.show();

    return a.exec();
}
