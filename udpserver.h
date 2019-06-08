#ifndef UdpServer_H
#define UdpServer_H

#include <QWidget>
#include <QUdpSocket>
#include "QTextEdit"
#include "QLineEdit"
#include "QCheckBox"
#include "QTimer"

class UdpServer : public QWidget
{
    Q_OBJECT
public:
    explicit UdpServer(int port,int wport,QString IP, QWidget* pwgt = 0);
    int WritePort, ReadPort;
    QCheckBox* check;
    QTextEdit*  m_ptxtInfo;
    QLineEdit*  m_ptxtInput;
    QString IP_Adress;
    QTimer AutoMessageTimer;
    int Count;
    int Current;
    QStringList str;

signals:

public slots:
    void AutoMsg();
    void SentData();
    void readyRead();

private:
    QUdpSocket *socket;

};

#endif // UdpServer_H
