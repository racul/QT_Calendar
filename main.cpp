#include "mainwindow.h"

#include <QApplication>

// 프로그램 실행 진입점

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
