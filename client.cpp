#include "client.h"
#include "QTextEdit"
#include "QLineEdit"
#include "QPushButton"
#include "QLabel"
#include "QVBoxLayout"
#include "QTime"
#include "QTimer"


MyClient::MyClient(const QString& strHost,int nPort,QWidget* pwgt) : QWidget(pwgt)
  , m_nNextBlockSize(0)
{
    // СОЗДАНИЕ И УСТАНОВКА ПРЕСЕТОВ
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(strHost, nPort);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError))
            );

    // СОЗДАНИЕ ВИДЖЕТОВ
    check = new QCheckBox;
    m_ptxtInfo  = new QTextEdit;
    m_ptxtHistory = new QTextEdit;
    m_ptxtInput = new QLineEdit;

    connect(m_ptxtInput, SIGNAL(returnPressed()),
            this,        SLOT(slotSendToServer())
            );
    check->setChecked(0);
    m_ptxtInfo->setReadOnly(true);
    m_ptxtHistory->setReadOnly(true);

    QPushButton* pcmd = new QPushButton("&Отправить сообщение");
    connect(pcmd, SIGNAL(clicked()),
            SLOT(slotSendToServer()));

    QLabel* bla =new QLabel("Автоматическая говорилка:");


    //СОЗДАНИЕ ОБЛАСТИ ВИДЖЕТА И РАССТАНОВКА СОЗДАННЫХ ВИДЖЕТОВ ИХ В ОБЛАСТИ
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    setGeometry(500,200,500,800);
    pvbxLayout->addWidget(new QLabel("Входящие сообщения:"));
    pvbxLayout->addWidget(m_ptxtInfo);
    pvbxLayout->addWidget(new QLabel("Исходящие сообщения:"));
    pvbxLayout->addWidget(m_ptxtHistory);
    pvbxLayout->addWidget(m_ptxtInput);
    pvbxLayout->addWidget(pcmd);
    pvbxLayout->addWidget(bla);
    pvbxLayout->addWidget(check);
    setLayout(pvbxLayout);
    tick = new QTimer;
    tick->setInterval(4500);
    tick->start();
    connect(tick,SIGNAL(timeout()),this,SLOT(AutoMes()));
}

void MyClient::slotReadyRead() // ФУНКЦИЯ СОЗДАНИЯ СЛОТА  ЧТЕНИЯ СЕРВЕРА
{
    QString str=m_pTcpSocket->readAll();

    m_ptxtInfo->append(str);
}

void MyClient::slotError(QAbstractSocket::SocketError err) // ФУНКЦИЯ ВЫДАЮЩАЯ ОШИБКИ
{
    QString strError =
            "Ошибка: " + (err == QAbstractSocket::HostNotFoundError ?
                              "Сервер не был найден." :
                              err == QAbstractSocket::RemoteHostClosedError ?
                                  "Хост в данный момент закрыт." :
                                  err == QAbstractSocket::ConnectionRefusedError ?
                                      "Соединение потеряно." :
                                      QString(m_pTcpSocket->errorString())
                                      );
    m_ptxtInfo->append(strError);
}

void MyClient::slotSendToServer() // ФУНКЦИЯ ОТПРАВЛЕНИЯ СООБЩЕНИЙ КЛИЕНТУ
{
    if(m_ptxtInput->text() != "/n" and m_ptxtInput->text() != "/n/n" and m_ptxtInput->text() != "" )
    {
        m_ptxtHistory->append(m_ptxtInput->text());

        QString temp = m_ptxtInput->text();
        QByteArray arr;
        arr.append(temp);
        m_pTcpSocket->write(arr);
        m_ptxtInput->setText("");
    }
}

void MyClient::slotConnected() // ФУНКЦИЯ ОПОВЕЩАЮЩАЯ ОБ УСПЕШНОМ ПОДКЛЮЧЕНИИ С СЕРВЕРОМ
{
    m_ptxtInfo->append("Подключение к серверу произошло успешно.");
}

void MyClient::AutoMes() // БОЛТАЛКА
{
    if(check != nullptr)
    {
        if(check->isChecked())
        {
            QString str = m_ptxtInput->text();
            m_ptxtInput->setText(QString::number(qrand()%2000));
            slotSendToServer();
            m_ptxtInput->setText(str);
        }
    }
}
