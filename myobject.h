#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>
#include <QTime>
#include <QTextBrowser>
#include <QTimer>
#include <QtNetwork>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "common.h"

class MyObject
{
public:
    explicit MyObject();
    virtual ~MyObject();
    virtual void sendMsg(QString str,QString addr="")=0;
    virtual void sendHexMsg(QString msg,QString addr="")=0;
    virtual void timerSend(QString msg, float s, int count,int sendType,QString add="")=0;
    void stopTimer();
    void clearData();
    void saveData();
    void setDeviceType(int type);
    int getDeviceType();
    QString getMyIP();
    QString getOtherIP();
    int getMyPort();
    int getOtherPort();
    int getRicvBit();
    int getSendBit();
    int getflagcount();
    void setZero();
    bool isTimerSending();
    void clearSend(int type);
    void loadFile(QString path, int type);

private:
    void findMyIP();

public:
    //设备输入输出框、16进制输入框
    QTextBrowser *textBrowser;
    QTextEdit *textEdit;
    common *hexEdit;
    bool isFile=false;

protected:
    //设备属性
    QString myIP;
    QString otherIP;
    int myPort=0;
    int otherPort=0;
    int falgcount=-1;
    int type=-1;

    //接收发送字节数
    int ricvBit=0;
    int sendBit=0;

    //定时发送
    QTimer *m_timer;
    int timerCount=0;
    int nowCount=0;
    QString addr;
    QString msg;
    int sendType=0;
    bool isTimeSending=false;
    QByteArray data;

};

#endif // MYOBJECT_H
