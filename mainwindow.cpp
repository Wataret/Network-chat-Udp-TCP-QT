#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setInputMask("00.00.000.000"); // ПРЕСЕТ НА СТРОКУ IP АДРЕСС
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_TcpServer_clicked() // ОТКРЫТИЯ  ВИДЖЕТА СЕРВЕР ПРИ НАЖАТИИ НА КНОПКУ
{
    server = new MyServer(ui->spinBox->value());
    this->hide();
    server->show();
}

void MainWindow::on_TcpClient_clicked() // ОТКРЫТИЯ  ВИДЖЕТА КЛИЕНТ ПРИ НАЖАТИИ НА КНОПКУ
{
    client = new MyClient(ui->lineEdit->text(),ui->spinBox->value());
    this->hide();
    client->show();
}

void MainWindow::on_Udp_clicked() // ОТКРЫТИЯ ВИДЖЕТА UDP ПРИ НАЖАТИИ НА КНОПКУ
{
    udps = new UdpServer(ui->spinBox->value(),ui->spinBox_2->value(),ui->lineEdit->text());
    this->hide();
    udps->show();
}
