#ifndef _MyClient_h_
#define _MyClient_h_

#include <QWidget>
#include <QTcpSocket>
#include "QCheckBox"

class QTextEdit;
class QLineEdit;

class MyClient : public QWidget
{
    Q_OBJECT

private:
    QCheckBox* check;
    QTcpSocket* m_pTcpSocket;
    QTextEdit*  m_ptxtInfo;
    QTextEdit*  m_ptxtHistory;
    QLineEdit*  m_ptxtInput;
    quint16     m_nNextBlockSize;
    QTimer *tick;

public:
    MyClient(const QString& strHost, int nPort, QWidget* pwgt = 0) ;

private slots:
    void AutoMes();
    void slotReadyRead();
    void slotError (QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
};
#endif  //_MyClient_h_
