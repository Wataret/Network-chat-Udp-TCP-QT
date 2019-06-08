#include "server.h"
#include "QMessageBox"
#include "QTextEdit"
#include "QVBoxLayout"
#include "QLabel"
#include "QPushButton"
#include "QTcpSocket"
#include "QTime"
#include "QHostAddress"
#include "QTimer"

MyServer::MyServer(int nPort, QWidget* pwgt) : QWidget(pwgt)
  , m_nNextBlockSize(0)
{
    // СОЗДАНИЕ И УСТАНОВКА ПРЕСЕТОВ
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) // ПРЕДПРЕЖДЕНИЕ ОШИБКИ СЕРВЕРА
    {
        QMessageBox::critical(0,
                              "Ошибка сервера",
                              "Невозможно запустить сервер:"
                              + m_ptcpServer->errorString()
                              );
        m_ptcpServer->close();
        return;
    }

    connect(m_ptcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection())
            );

    // СОЗДАНИЕ ВИДЖЕТОВ
    check = new QCheckBox;
    check->setChecked(0);

    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);

    m_ptxtHistory = new QTextEdit;
    m_ptxtHistory->setReadOnly(true);

    m_ptxt_input = new QLineEdit;
    m_ptxt_input->move(25,200);


    QPushButton* send = new QPushButton("&Отправить сообщение");
    connect(send, SIGNAL(clicked()),
            SLOT(sendToClient()));

    QLabel* bla =new QLabel("Автоматическая говорилка:");

    //СОЗДАНИЕ ОБЛАСТИ ВИДЖЕТА И РАССТАНОВКА СОЗДАННЫХ ВИДЖЕТОВ ИХ В ОБЛАСТИ
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    setGeometry(500,200,500,800);
    pvbxLayout->addWidget(new QLabel("Входящие сообщения:"));
    pvbxLayout->addWidget(m_ptxt);
    pvbxLayout->addWidget(new QLabel("Исходящие сообщения:"));
    pvbxLayout->addWidget(m_ptxtHistory);
    pvbxLayout->addWidget(m_ptxt_input);
    pvbxLayout->addWidget(send);
    pvbxLayout->addWidget(bla);
    pvbxLayout->addWidget(check);
    setLayout(pvbxLayout);
    socket = 0;
    socket2 = 0 ;
    tick = new QTimer;
    tick->setInterval(5000);
    tick->start();
    connect(tick,SIGNAL(timeout()),this,SLOT(AutoMes()));
}

void MyServer::slotNewConnection() // ФУНКЦИЯ СОЗДАНИЯ СЛОТА  СОЕДИНЕНИЯ
{
    socket = m_ptcpServer->nextPendingConnection();


    connect(socket, SIGNAL(disconnected()),
            socket, SLOT(deleteLater())
            );
    connect(socket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
            );

    sendToClient(socket,"Подключение к клиенту произошло успешно.");



}

void MyServer::slotReadClient() // ФУНКЦИЯ СОЗДАНИЯ СЛОТА  ЧТЕНИЯ КЛИЕНТА
{
    socket2 = (QTcpSocket*)sender();

    QString str=socket2->readAll();

    m_ptxt->append(str);

}

void MyServer::sendToClient(QTcpSocket* pSocket, const QString& str) // ФУНКЦИЯ ОГРАНИЧИВАЮЩАЯ ОТПРАВЛЕНИЯ СООБЩЕНИЙ КЛИЕНТУ
{

    m_ptxtHistory->append(str);
    QString old = m_ptxt_input->text();
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    if(pSocket != 0)
    {
        pSocket->write(arrBlock);
    }
    m_ptxt_input->setText(old);
}

void MyServer::sendToClient() // ФУНКЦИЯ ОТПРАВЛЕНИЯ СООБЩЕНИЙ КЛИЕНТУ
{

    if(m_ptxt_input->text() != "/n" and m_ptxt_input->text() != "/n/n" and m_ptxt_input->text() != "" )
    {

        m_ptxtHistory->append(m_ptxt_input->text());

        QString temp = m_ptxt_input->text();
        QByteArray arr;
        arr.append(temp);
        socket->write(arr);
        m_ptxt_input->setText("");
    }
}

void MyServer::AutoMes() // Болталка
{
    if(check != nullptr)
    {
        if(check->isChecked())
        {
            sendToClient(socket, QString::number(qrand()));
        }
    }
}
