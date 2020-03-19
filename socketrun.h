#ifndef SOCKETRUN_H
#define SOCKETRUN_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include "common.h"
class socketRun : public QObject
{
    Q_OBJECT
public:
    explicit socketRun(int count,qintptr socketDescriptor, QObject *parent = nullptr);

    void readSlot();
    bool sendSucc=false;
    void write(QByteArray ba);
    QString getShowIp();
    void setShowIP(QString ip);
    QString RealyIP();
    int getInComBoboxIndex();
    void setInComBoboxIndex(int index);
    bool isConnect();


private:
    int count;
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QString clientip;
    QString showIP;
    int inComBoboxIndex=-1;
    bool isConn=true;



signals:
    void readSocketSignal(QByteArray ba,int size, QString ip);
    void started();
    void returnSendBitSignal(int num);
    void writeSIGNAL(QByteArray ba);
    void disConnectSIGNAL(int count);
    void runOver(socketRun *sock);
    void sendNewSendBit(int size);
    void updateSend(int sendBit);


public slots:
    void writeSLOT(QByteArray ba);
    void disConnSLOT();
    void run();
};

#endif // SOCKETRUN_H
