#ifndef _MyServer_h_
#define _MyServer_h_

#include <QWidget>
#include <QTcpServer>
#include "QLineEdit"
#include "QCheckBox"

class QTcpServer;
class QTextEdit;
class QTcpSocket;

class MyServer : public QWidget {

    Q_OBJECT

private:
    QCheckBox* check;
    QTcpServer* m_ptcpServer;
    QTextEdit*  m_ptxt;
    QTextEdit*  m_ptxtHistory;
    QLineEdit* m_ptxt_input;
    quint16     m_nNextBlockSize;
    QTcpSocket* socket,*socket2;
    bool buton=false;

private:

public:
    MyServer(int nPort, QWidget* pwgt = 0);
    void sendToClient(QTcpSocket* pSocket, const QString& str);
    QTimer *tick;

public slots:
    void AutoMes();
    void sendToClient();
    virtual void slotNewConnection();
    void slotReadClient   ();
};
#endif  //_MyServer_h_
