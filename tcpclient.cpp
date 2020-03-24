#include "tcpclient.h"

tcpClient::tcpClient(QString ip, int port, int falgcount, QObject *parent) : QObject (parent)
{
    socket=new QTcpSocket;
    type=2;//设置设备是tcp客户端
    otherIP=ip;
    otherPort=port;
    this->falgcount=falgcount;

    connect(socket,&QTcpSocket::readyRead,this,&tcpClient::ricvMsgSLOT);
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnectSLOT()));
}

tcpClient::~tcpClient()
{
    qDebug()<<"删除客户端";
    textEdit->deleteLater();
    textBrowser->deleteLater();
}

bool tcpClient::connectToHost()
{
    socket->abort();
    socket->connectToHost(QHostAddress(otherIP),quint16(otherPort));
    if(!socket->waitForConnected(2000)){//等待连接
        socket->disconnectFromHost();//如果超时，连接失败
        //QMessageBox::warning("失败","连接超时！");
        isConnection=false;
        return false;
    }
    qDebug()<<"客户端connectToHost1";
    isConnection=true;
    return true;
}

void tcpClient::disConnectToHost()
{
    socket->abort();
    socket->close();
    isConnection=false;
}

void tcpClient::sendMsg(QString str, QString addr)
{
    if(!socket->isOpen())
        return;
    data.clear();
    if(isFile)
    {
        QStringList strlist=textEdit->toPlainText().split("[");
        QStringList strlist2=strlist.at(1).split("]");
        qDebug()<<strlist.length()<<strlist2.at(0);

        QFile file(strlist2.at(0));
        if(!file.open(QIODevice::ReadWrite))
        {
            QMessageBox::warning(nullptr,"ERROR","打开外部数据源文件失败！",nullptr,nullptr);
            return;
        }
        data=file.readAll();
    }
    else {
        data=str.toLocal8Bit();
    }

    int size=int(socket->write(data,data.size()));
    if(size>=0)
    {
        sendBit+=size;
        QString str1="["+common::getCurrTime()+"]发送[ASCII]："+str;
        textBrowser->append(str1);
    }
    emit sendMsgSIGNAL(sendBit,ricvBit,falgcount,type);
}

void tcpClient::sendHexMsg(QString msg,QString addr)
{
    if(!socket->isOpen())
        return;
    data.clear();
    if(isFile)
    {
        QStringList strlist=hexEdit->toPlainText().split("[");
        QStringList strlist2=strlist.at(1).split("]");
        QFile file(strlist2.at(0));
        if(!file.open(QIODevice::ReadWrite))
        {
            QMessageBox::warning(nullptr,"ERROR","打开外部数据源文件失败！",nullptr,nullptr);
            return;
        }
        QByteArray ba=file.readAll();
        QString strHex=data.toHex().toStdString().c_str();
        data=common::hexStrToByteArray(strHex.toUpper());
    }
    else {
        data=common::hexStrToByteArray(msg);
    }


    int size=int(socket->write(data,data.size()));
    if(size>=0)
    {
        sendBit+=size;
        QString str1="["+common::getCurrTime()+"]发送[Hex]："+msg;
        textBrowser->append(str1);
    }
    emit sendMsgSIGNAL(sendBit,ricvBit,falgcount,type);
}

void tcpClient::disconnectSLOT()
{
    qDebug()<<"客户端与服务器断开连接";
    socket->abort();
    socket->close();
    isConnection=false;
    emit disconnectSIGNAL();
}

void tcpClient::ricvMsgSLOT()
{

    QByteArray ba;
    ba=socket->readAll();
    QString str=QString::fromLocal8Bit(ba);
    qDebug()<<"ba="<<ba;
    qDebug()<<"str="<<str;
    //QString strHex=ba.toHex().toStdString().c_str();
    QString strHex=QString::fromLocal8Bit(ba.toHex());
    int size=ba.size();
    qDebug()<<"客户端ricv:"<<size;
    ricvBit+=size;
    QString str1="["+common::getCurrTime()+"]"+otherIP+":["+strHex.toUpper()+"]"+str;
    textBrowser->append(str1);
    emit sendMsgSIGNAL(sendBit,ricvBit,falgcount,type);
}

void tcpClient::timerSend(QString msg, float s, int count, int sendType, QString add)
{
    m_timer=new QTimer();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerSendSLOT()));
    int ms=s*1000;
    qDebug()<<"s="<<s;
    qDebug()<<"ms="<<ms;
    m_timer->setInterval(ms);
    this->timerCount=count;
    this->sendType=sendType;
    this->msg=msg;
    isTimeSending=true;
    m_timer->start();
}

void tcpClient::timerSendSLOT()
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
            emit sendMsgSIGNAL(sendBit,ricvBit,falgcount,type);
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
            emit sendMsgSIGNAL(sendBit,ricvBit,falgcount,type);
            m_timer->stop();
            nowCount=0;
            delete m_timer;
            m_timer=nullptr;
            return;
        }
    }
}
