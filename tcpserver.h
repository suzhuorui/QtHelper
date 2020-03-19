#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include "myobject.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QtNetwork>
#include <QNetworkInterface>
#include <QHostAddress>
#include<QThread>
#include "socketrun.h"

class tcpServer : public QTcpServer,public MyObject
{
    Q_OBJECT
public:
    explicit tcpServer(int falgcount,QObject *parent = nullptr);
    ~tcpServer();
    bool getState();
    bool listenTcp();
    void closeListen();
    QStringList getClientList();
    virtual void sendMsg(QString str,QString addr="");
    virtual void sendHexMsg(QString msg, QString addr="");
    virtual void timerSend(QString msg, float s, int count, int sendType, QString add="");
    void RemoveClient(int index);
    void setPort(int port);
    bool sendSucc=false;

private:
    QTcpSocket *socket;
    QStringList clientList;
    QList<socketRun*> socketList;
    //监听状态
    bool isListen=false;
    QString addr;

protected:
    virtual void incomingConnection(qintptr socketDescriptor);
signals:
    void readSocketSignal(QByteArray ba);
    void clientMore();
    void hasClient(QString addr,tcpServer &server);
    void sendDateSIGNAL(int,int,int,int);
    void sendDisSIGNAL();

public slots:
    void readslot(QByteArray ba, int size, QString ip);
    void disConnSLOT(int index);
    void getClientInfo(socketRun *sock);
    void upDateSendBit(int size);
    void timerSendSLOT();
};

#endif // TCPSERVER_H
