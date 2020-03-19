#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include "myobject.h"

class udpClient : public QObject,public MyObject
{
    Q_OBJECT
public:
    explicit udpClient(QString clientIP, int clientPort, int falgcount,QObject *parent = nullptr);
    virtual void sendMsg(QString msg,QString addr="");
    virtual void sendHexMsg(QString msg,QString addr="");
    virtual void timerSend(QString msg, float s, int count, int sendType, QString add="");

private:
    QUdpSocket *m_socket;

signals:
    emit void ricvBitSIGNAL(int,int,int,int);

public slots:
    void ReadyreadSLOT();
    void timerSendSLOT();
};

#endif // UDPCLIENT_H
