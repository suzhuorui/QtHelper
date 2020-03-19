#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include "myobject.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QtNetwork>
#include <QNetworkInterface>
class tcpClient : public QObject,public MyObject
{
    Q_OBJECT
public:
    explicit tcpClient(QString ip,int port,int falgcount,QObject *parent = nullptr);
    ~tcpClient();
    bool connectToHost();
    void disConnectToHost();
    virtual void sendMsg(QString str,QString addr="");
    virtual void sendHexMsg(QString msg,QString addr="");
    virtual void timerSend(QString msg, float s, int count, int sendType, QString add="");
    bool isConnection=false;
    int hostFlagCount;

private:
    QTcpSocket *socket;

signals:
    void disconnectSIGNAL();
    void sendMsgSIGNAL(int,int,int,int);

public slots:
    virtual void ricvMsgSLOT();
    void disconnectSLOT();
    void timerSendSLOT();
};

#endif // TCPCLIENT_H
