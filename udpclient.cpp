#include "udpclient.h"

//UDP客户端：向指定IP和端口发送信息。接收所有传向本端口的数据，但不分析。

udpClient::udpClient(QString clientIP, int clientPort, int falgcount, QObject *parent) : QObject(parent),MyObject ()
{
    type=4;
    otherIP=clientIP;
    otherPort=clientPort;
    this->falgcount=falgcount;
    m_socket=new QUdpSocket(this);
    connect(m_socket,&QUdpSocket::readyRead,this,&udpClient::ReadyreadSLOT);
}

void udpClient::sendMsg(QString msg,QString addr)
{
    QByteArray ba=msg.toLocal8Bit();
    //qDebug()<<msg.setNum(msg.toInt(),16);
    //qDebug()<<ba.toHex().toStdString().c_str();
    int len=m_socket->writeDatagram(ba,QHostAddress(otherIP),quint16(otherPort));
    if(len>0)
    {
        sendBit+=len;
        QString str="["+common::getCurrTime()+"]发送[Hex]："+msg;
        textBrowser->append(str);
        qDebug()<<sendBit<<ricvBit;
        emit ricvBitSIGNAL(sendBit,ricvBit,falgcount,type);
    }
}

void udpClient::sendHexMsg(QString msg, QString addr)
{
    QByteArray ba=common::hexStrToByteArray(msg);
    int len=m_socket->writeDatagram(ba,QHostAddress(otherIP),quint16(otherPort));
    if(len>0)
    {
        sendBit+=len;
        QString str="["+common::getCurrTime()+"]发送[Hex]："+msg;
        textBrowser->append(str);
        qDebug()<<sendBit<<ricvBit;
        emit ricvBitSIGNAL(sendBit,ricvBit,falgcount,type);
    }
}

void udpClient::ReadyreadSLOT()
{
    QByteArray ba;
    QHostAddress addr;
    quint16 p;
    ba.resize(m_socket->pendingDatagramSize());
    qint64 len=m_socket->readDatagram(ba.data(),ba.size(),&addr,&p);
    if(len>0)
    {
        QString str=QString::fromLocal8Bit(ba);
        QString strHex=ba.toHex().toStdString().c_str();
        QString str1="["+common::getCurrTime()+"]收到:["+strHex.toUpper()+"]"+str;
        textBrowser->append(str1);
        ricvBit+=len;
        emit ricvBitSIGNAL(sendBit,ricvBit,falgcount,type);
    }
}

void udpClient::timerSend(QString msg, float s, int count, int sendType, QString add)
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

void udpClient::timerSendSLOT()
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
