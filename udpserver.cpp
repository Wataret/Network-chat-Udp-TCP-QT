#include "udpserver.h"
#include "QTextEdit"
#include "QVBoxLayout"
#include "QLabel"
#include "QPushButton"
#include "QTime"
#include "QDebug"
#include "QThread"

UdpServer::UdpServer(int port,int wport, QString IP, QWidget* pwgt) : QWidget(pwgt)
{
    // СОЗДАНИЕ И УСТАНОВКА ПРЕСЕТОВ
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    socket = new QUdpSocket(this);
    WritePort = wport;
    ReadPort = port;
    IP_Adress = IP;

    socket->bind(QHostAddress::Any, ReadPort);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    check = new QCheckBox;
    m_ptxtInfo  = new QTextEdit;
    m_ptxtInput = new QLineEdit;

    connect(m_ptxtInput, SIGNAL(returnPressed()),
            this,        SLOT(SentData())
            );

    m_ptxtInfo->setReadOnly(true);
    check->setChecked(0);

    // СОЗДАНИЕ ВИДЖЕТОВ
    QPushButton* pcmd = new QPushButton("&Отправить сообщение");
    connect(pcmd, SIGNAL(clicked()), SLOT(SentData()));
    connect(&AutoMessageTimer,SIGNAL(timeout()),this,SLOT(AutoMsg()));
    AutoMessageTimer.start(2500);

    QLabel* bla =new QLabel("Автоматическая говорилка:");

    //СОЗДАНИЕ ОБЛАСТИ ВИДЖЕТА И РАССТАНОВКА СОЗДАННЫХ ВИДЖЕТОВ ИХ В ОБЛАСТИ
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("Входящие сообщения:"));
    pvbxLayout->addWidget(m_ptxtInfo);
    pvbxLayout->addWidget(m_ptxtInput);
    pvbxLayout->addWidget(pcmd);
    pvbxLayout->addWidget(bla);
    pvbxLayout->addWidget(check);
    setLayout(pvbxLayout);
    Count = 0;
}

void UdpServer::SentData() // ФУНЦИЯ ТРАНСПОРТИРОВКИ ДАННЫХ
{
    QByteArray Data;
    QStringList str;
    QString temp = m_ptxtInput->text();
    for(int i = 0; temp.count()>0;i++)
    {
        QString t = temp.mid(0,800);
        temp.remove(0,800);
        str.push_back(t);
    }
    if(str.count() == 1)
    {
        Data.append("000");
        Data.append(str.at(0));
        socket->writeDatagram(Data, QHostAddress(IP_Adress), WritePort);
    }
    else
    {
        Data.clear();

        if(str.count()<10)
            Data.append("00"+QString::number(str.count()));
        else if(str.count()<100)
            Data.append("0"+QString::number(str.count()));
        else if(str.count()<1000)
            Data.append(QString::number(str.count()));

        socket->writeDatagram(Data, QHostAddress(IP_Adress), WritePort);
        for(int i = 0; i< str.count();i++)
        {
            Data.clear();

            if(i<10)
                Data.append("00"+QString::number(i));
            else if(i<100)
                Data.append("0"+QString::number(i));
            else if(i<1000)
                Data.append(QString::number(i));

            Data.append(str.at(i));
            thread()->msleep(5);
            socket->writeDatagram(Data, QHostAddress(IP_Adress), WritePort);
        }
    }
    m_ptxtInput->clear();
}

void UdpServer::readyRead() // ФУНКЦИЯ ДЛЯ ПРИЕМА ИНФОРМАЦИИ
{
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);
    if(buffer.mid(0,3).toInt() == 0 && Count == 0)
    {
        m_ptxtInfo->append(buffer.mid(3));
    }
    else if(buffer.size()<=6)
    {
        Count = buffer.mid(0,3).toInt();
        Current = 0;
        str.clear();
        for(int i = 0; i< Count; i++)
        {
            str.push_back(" ");
        }
    }
    else
    {
        Current++;
        int cur = buffer.mid(0,3).toInt();
        if(str.at(cur).count() <= 3)
        {
            str.takeAt(cur);
            str.insert(cur,buffer.mid(3));
        }
    }
    if(Current == Count)
    {
        QString result;
        for(int i = 0; i <Count; i++)
        {
            result.append(str.at(i));
        }
        m_ptxtInfo->append(result);
        Count = 0;
    }
}

void UdpServer::AutoMsg() // БОЛТАЛКА
{
    QByteArray Data;
    Data.append("000");
    Data.append(QString::number(qrand()));
    if(check != nullptr)
    {
        if(check->isChecked())
        {
            socket->writeDatagram(Data, QHostAddress(IP_Adress), WritePort);
        }
    }
}
