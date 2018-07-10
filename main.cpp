#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Suppose v3.0 Simulation File: *new file*");
    w.show();

    return a.exec();
}
