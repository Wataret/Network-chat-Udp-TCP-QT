#include "mainwindow.h"
#include <QApplication>
#include "server.h"
#include "client.h"
#include "udpserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
