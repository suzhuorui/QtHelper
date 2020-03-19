#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include "myobject.h"

class udpServer : public QObject,public MyObject
{
    Q_OBJECT
public:
    explicit udpServer(int port,int falgcount, QObject *parent = nullptr);
    virtual void sendMsg(QString msg,QString addr="");
    virtual void sendHexMsg(QString msg,QString addr="");
    virtual void timerSend(QString msg, float s, int count,int sendType,QString add="");

private:
    QUdpSocket *m_socket;

signals:
    void ricvBitSIGNAL(int,int,int,int);

public slots:
    void ReadyreadSLOT();
    void timerSendSLOT();
};

#endif // UDPSERVER_H
