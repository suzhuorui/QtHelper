#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "myobject.h"

class SerialPort : public QSerialPort,public MyObject
{
    Q_OBJECT
public:
    explicit SerialPort(QString name,int falgcount,QObject *parent = nullptr);
    bool openSerialPort(int strBaudRate, int strDataBits, int strParity, int strStopBits);
    bool getState();
    void closeSerialPort();
    virtual void sendMsg(QString msg,QString addr="");
    virtual void sendHexMsg(QString msg,QString addr="");
    virtual void timerSend(QString msg, float s, int count, int sendType,QString add="");

    int strBaudRate;
    int strDataBits;
    int strParity;
    int strStopBits;


private:
    QSerialPort *m_serial;

    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
    void findSerialPort();
    void getSetting(int strBaudRate, int strDataBits, int strParity, int strStopBits);


signals:
    void ricvBitSIGNAL(int,int,int,int);

public slots:
    void ReadyreadSLOT();
    void timerSendSLOT();
};

#endif // SERIALPORT_H
