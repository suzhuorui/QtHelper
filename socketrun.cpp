#include "socketrun.h"
#include <QThread>
socketRun::socketRun(int count, qintptr socketDescriptor, QObject *parent) : QObject(parent)
{
    this->count=count;
    this->socketDescriptor=socketDescriptor;
    qDebug()<<"构造"<<QThread::currentThreadId();
}

void socketRun::run(){
    qDebug()<<"run";
    socket=new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    clientip=socket->peerAddress().toString();
    showIP=clientip;
    connect(this,&socketRun::writeSIGNAL,this,&socketRun::writeSLOT);//,Qt::BlockingQueuedConnection
    connect(socket,&QTcpSocket::readyRead,this,&socketRun::readSlot);
    connect(socket,&QTcpSocket::disconnected,this,&socketRun::disConnSLOT);
    emit runOver(this);
}

void socketRun::readSlot()
{
    qDebug()<<"readSlot"<<QThread::currentThreadId();
    QByteArray ba=socket->readAll();
    int size=ba.size();
    emit readSocketSignal(ba,size,showIP);
}

//在自己的线程中调用自己的私有对象函数，避免跨线程错误
void socketRun::write(QByteArray ba)
{
    qDebug()<<"write";
    emit writeSIGNAL(ba);
}

QString socketRun::getShowIp()
{
    return showIP;
}

void socketRun::setShowIP(QString ip)
{
    this->showIP=ip;
}

QString socketRun::RealyIP()
{
    return clientip;
}

int socketRun::getInComBoboxIndex()
{
    return inComBoboxIndex;
}

void socketRun::setInComBoboxIndex(int index)
{
    this->inComBoboxIndex=index;
}

bool socketRun::isConnect()
{
    return isConn;
}

void socketRun::writeSLOT(QByteArray ba)
{
    qDebug()<<"writeSLOT:"<<QThread::currentThreadId();
    int size=int(socket->write(ba,ba.size()));
    if(size>=0)
    {
        qDebug()<<"writeSLOT:"<<size;
        emit sendNewSendBit(size);
    }
    else {
        //服务器发送失败;
    }
}

void socketRun::disConnSLOT()
{
    isConn=false;
    emit disConnectSIGNAL(count);
}
