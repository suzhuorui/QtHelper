#include "serialport.h"
#include <QDebug>
SerialPort::SerialPort(QString name,int falgcount, QObject *parent) : QSerialPort(parent)
{
    this->falgcount=falgcount;
    this->name=name;
    m_serial=new QSerialPort(this);
    type=5;
    this->strBaudRate=3;
    this->strDataBits=3;
    this->strParity=1;
    this->strStopBits=0;
    connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReadyreadSLOT()));
}

bool SerialPort::openSerialPort(int strBaudRate,int strDataBits,int strParity,int strStopBits)
{
    getSetting(strBaudRate,strDataBits,strParity,strStopBits);
    m_serial->setPortName(name);
    bool isOpen=m_serial->open(QIODevice::ReadWrite);
    if(isOpen)
    {
        m_serial->setBaudRate(baudRate);//9600
        m_serial->setDataBits(dataBits);//8位
        m_serial->setParity(parity);//奇校验     EvenParity
        m_serial->setStopBits(stopBits);//一位停止位
        m_serial->setFlowControl(QSerialPort::NoFlowControl);//没有流

        return isOpen;
    }
    return isOpen;
}

bool SerialPort::getState()
{
    return m_serial->isOpen();
}

void SerialPort::closeSerialPort()
{
    m_serial->close();
}

void SerialPort::sendMsg(QString msg,QString addr)
{
    QByteArray ba=msg.toLocal8Bit();
    int len=m_serial->write(ba);
    if(len>0)
    {
        sendBit+=len;
        QString str1=getBlueString("["+common::getCurrTime()+"]")+"发送"+getGreenString("[ASCII]:")+getRedString(msg);
        //QString str="["+common::getCurrTime()+"]发送[ASCII]："+msg;
        textBrowser->append(str1);
        emit ricvBitSIGNAL(sendBit,ricvBit,falgcount,type);
    }
}

void SerialPort::sendHexMsg(QString msg,QString addr)
{
    QByteArray ba=common::hexStrToByteArray(msg);
    int len=m_serial->write(ba);
    if(len>0)
    {
        sendBit+=len;
        QString str1=getBlueString("["+common::getCurrTime()+"]")+"发送"+getGreenString("[Hex]:")+getRedString(msg);
        //QString str="["+common::getCurrTime()+"]发送[Hex]："+msg;
        textBrowser->append(str1);
        emit ricvBitSIGNAL(sendBit,ricvBit,falgcount,type);
    }
}

void SerialPort::timerSend(QString msg, float s, int count, int sendType, QString add)
{
    qDebug()<<"5";
    m_timer=new QTimer();
    this->timerCount=count;
    this->msg=msg;
    this->sendType=sendType;
    int ms=s*1000;
    qDebug()<<"5.2";
    m_timer->setInterval(ms);
    qDebug()<<"5.3";
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerSendSLOT()));
    isTimeSending=true;
    m_timer->start();
    qDebug()<<"5.7";
}

//Index   Index    Index   Index
void SerialPort::getSetting(int strBaudRate,int strDataBits,int strParity,int strStopBits)
{
    qDebug()<<strBaudRate<<strDataBits<<strParity<<strStopBits;
    this->strBaudRate=strBaudRate;
    this->strDataBits=strDataBits;
    this->strParity=strParity;
    this->strStopBits=strStopBits;
    switch (strBaudRate) {
    case 0:
        baudRate=QSerialPort::Baud1200;
        qDebug()<<"设置波特率1200";
        break;
    case 1:
        baudRate=QSerialPort::Baud2400;
        qDebug()<<"设置波特率Baud2400";
        break;
    case 2:
        baudRate=QSerialPort::Baud4800;
        qDebug()<<"设置波特率Baud4800";
        break;
    case 3:
        baudRate=QSerialPort::Baud9600;
        qDebug()<<"设置波特率Baud9600";
        break;
    case 4:
        baudRate=QSerialPort::Baud19200;
        qDebug()<<"设置波特率Baud19200";
        break;
    case 5:
        baudRate=QSerialPort::Baud38400;
        qDebug()<<"设置波特率Baud38400";
        break;
    case 6:
        baudRate=QSerialPort::Baud57600;
        qDebug()<<"设置波特率Baud57600";
        break;
    case 7:
        baudRate=QSerialPort::Baud115200;
        qDebug()<<"设置波特率Baud115200";
        break;
    }
    switch (strDataBits) {
    case 0:
        dataBits=QSerialPort::Data5;
        qDebug()<<"设置数据位5";
        break;
    case 1:
        dataBits=QSerialPort::Data6;
        qDebug()<<"设置数据位6";
        break;
    case 2:
        dataBits=QSerialPort::Data7;
        qDebug()<<"设置数据位7";
        break;
    case 3:
        dataBits=QSerialPort::Data8;
        qDebug()<<"设置数据位8";
        break;
    }
    switch (strParity) {
    case 0:
        parity=QSerialPort::NoParity;
        qDebug()<<"设置无校验";
        break;
    case 1:
        parity=QSerialPort::EvenParity;
        qDebug()<<"设置奇校验";
        break;
    case 2:
        parity=QSerialPort::OddParity;
        qDebug()<<"设置偶校验";
        break;
    case 3:
        parity=QSerialPort::SpaceParity;
        qDebug()<<"设置0校验";
        break;
    case 4:
        parity=QSerialPort::MarkParity;
        qDebug()<<"设置1校验";
        break;
    }
    switch (strStopBits) {
    case 0:
        stopBits=QSerialPort::OneStop;
        qDebug()<<"设置1停止";
        break;
    case 1:
        stopBits=QSerialPort::OneAndHalfStop;
        qDebug()<<"设置1.5停止";
        break;
    case 2:
        stopBits=QSerialPort::TwoStop;
        qDebug()<<"设置2停止";
        break;
    }
}

void SerialPort::ReadyreadSLOT()
{
    qDebug()<<"has msg";
    QByteArray ba=m_serial->readAll();
    if(ba.size()>0)
    {
        QString str=QString::fromLocal8Bit(ba);
        QString strHex=ba.toHex().toStdString().c_str();
        QString str1=getBlueString("["+common::getCurrTime()+"]")+otherIP+getGreenString("："+strHex.toUpper()+"]")+getRedString(str);
        //QString str1="["+common::getCurrTime()+"]收到:["+strHex.toUpper()+"]"+str;
        textBrowser->append(str1);
        ricvBit+=ba.size();
        emit ricvBitSIGNAL(sendBit,ricvBit,falgcount,type);
    }
}

void SerialPort::timerSendSLOT()
{
    qDebug()<<"6";
    if(sendType==10)
    {
        if(nowCount<timerCount)
        {
            qDebug()<<"7";
            nowCount++;
            sendMsg(msg);
        }
        else {
            isTimeSending=false;
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
            m_timer->stop();
            nowCount=0;
            delete m_timer;
            m_timer=nullptr;
            return;
        }
    }

}
