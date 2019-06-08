#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "udpserver.h"
#include "client.h"
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();
    UdpServer *udps;
    MyClient *client;
    MyServer *server;

private slots:
    void on_TcpServer_clicked();

    void on_TcpClient_clicked();

    void on_Udp_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
