﻿#include "udpserver.h"

//UDP服务器：监视自身端口，接收所有向本端口传来的数据，分析后得出对方IP和端口后，向对方发送消息。

udpServer::udpServer(int port, int falgcount, QObject *parent) : QObject(parent),MyObject ()
{
    myPort=port;
    type=3;
    this->falgcount=falgcount;
    m_socket=new QUdpSocket(this);
    m_socket->bind(QHostAddress(myIP),quint16(port));
    connect(m_socket,&QUdpSocket::readyRead,this,&udpServer::ReadyreadSLOT);
}

void udpServer::sendMsg(QString msg,QString addr)
{
    if(!otherIP.isEmpty())
    {
        QByteArray ba=msg.toLocal8Bit();
        int len=m_socket->writeDatagram(ba,QHostAddress(otherIP),quint16(otherPort));
        sendBit+=len;
        QString str="["+common::getCurrTime()+"]发送[ASCII]："+msg;
        textBrowser->append(str);
        emit ricvBitSIGNAL(sendBit,ricvBit,falgcount,type);
    }
}

void udpServer::sendHexMsg(QString msg, QString addr)
{
    if(!otherIP.isEmpty())
    {
        QByteArray ba=common::hexStrToByteArray(msg);
        int len=m_socket->writeDatagram(ba,QHostAddress(otherIP),quint16(otherPort));
        sendBit+=len;
        QString str="["+common::getCurrTime()+"]发送[Hex]："+msg;
        textBrowser->append(str);
        emit ricvBitSIGNAL(sendBit,ricvBit,falgcount,type);
    }
}

void udpServer::ReadyreadSLOT()
{
    QByteArray ba;
    QHostAddress addr;
    quint16 p;
    ba.resize(m_socket->pendingDatagramSize());
    int len=m_socket->readDatagram(ba.data(),ba.size(),&addr,&p);
    if(len>0)
    {
        otherIP=addr.toString();
        otherPort=int(p);
        QString str=QString::fromLocal8Bit(ba);
        QString strHex=ba.toHex().toStdString().c_str();
        QString str1="["+common::getCurrTime()+"]收到:["+strHex.toUpper()+"]"+str;
        textBrowser->append(str1);
        ricvBit+=len;
        emit ricvBitSIGNAL(sendBit,ricvBit,falgcount,type);
    }
}

void udpServer::timerSend(QString msg, float s, int count, int sendType, QString add)
{
    m_timer=new QTimer();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerSendSLOT()));
    int ms=s*1000;
    m_timer->setInterval(ms);
    this->timerCount=count;
    this->sendType=sendType;
    this->msg=msg;
    isTimeSending=true;
    m_timer->start();
}

void udpServer::timerSendSLOT()
{
    if(sendType==10)
    {
        if(nowCount<timerCount)
        {
            nowCount++;
            sendMsg(msg);
        }
        else {
            isTimeSending=false;
            emit ricvBitSIGNAL(sendBit,ricvBit,falgcount,type);
            m_timer->stop();
            nowCount=0;
            delete m_timer;
            m_timer=nullptr;
            return;
        }
    }
    else if(sendType==16){
        if(nowCount<timerCount)
        {
            nowCount++;
            sendHexMsg(msg);
        }
        else {
            isTimeSending=false;
            emit ricvBitSIGNAL(sendBit,ricvBit,falgcount,type);
            m_timer->stop();
            nowCount=0;
            delete m_timer;
            m_timer=nullptr;
            return;
        }
    }
}
