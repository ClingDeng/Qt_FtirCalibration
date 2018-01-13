#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("FTIR标定程序 V1.0");
    w.show();

    return a.exec();
}
