#include "myobject.h"

MyObject::MyObject()
{
    findMyIP();

    textBrowser=new QTextBrowser();
    textEdit=new QTextEdit();
    hexEdit=new common();
    hexEdit->setGeometry(3,20,471,151);
    textBrowser->setGeometry(5,20,611,191);
    textEdit->setGeometry(3,20,471,151);
    textEdit->setStyleSheet("background-color:transparent");
    textBrowser->setStyleSheet("background-color:transparent");
    hexEdit->setStyleSheet("background-color:transparent");
}

MyObject::~MyObject()
{
    textBrowser->deleteLater();
    textEdit->deleteLater();
    hexEdit->deleteLater();
}

void MyObject::setDeviceType(int type)
{
    this->type=type;
}

int MyObject::getDeviceType()
{
    return type;
}

QString MyObject::getMyIP()
{
    return myIP;
}

QString MyObject::getOtherIP()
{
    return otherIP;
}

int MyObject::getMyPort()
{
    return myPort;
}

int MyObject::getOtherPort()
{
    return otherPort;
}

int MyObject::getRicvBit()
{
    return ricvBit;
}

int MyObject::getSendBit()
{
    return sendBit;
}

int MyObject::getflagcount()
{
    return falgcount;
}

void MyObject::setZero()
{
    sendBit=0;
    ricvBit=0;
}

bool MyObject::isTimerSending()
{
    return isTimeSending;
}

void MyObject::findMyIP()
{
    QString ipaddress;
    QList<QHostAddress> list=QNetworkInterface::allAddresses();
    foreach(QHostAddress addre,list)
    {
        if(addre.protocol()==QAbstractSocket::IPv4Protocol)
        {
            if(addre.toString()=="127.0.0.1")
                continue;
            else {
                ipaddress=addre.toString();
                break;
            }
        }
    }
    myIP=ipaddress;
}

void MyObject::stopTimer()
{
    m_timer->stop();
    nowCount=0;
    isTimeSending=false;
    delete m_timer;
}

void MyObject::clearData()
{
    textBrowser->clear();
}

void MyObject::saveData()
{
    QString filePath = QFileDialog::getSaveFileName();
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"open file ERROR";
        return;
    }
    QString str=textBrowser->toPlainText();
    qDebug()<<str;
    QStringList strList=str.split("\n");
    foreach(QString str,strList)
    {
        str.append("\r\n");
        file.write(str.toUtf8());
    }
    file.close();
    QMessageBox::warning(nullptr,"消息","保存数据成功！（"+filePath+")",nullptr,nullptr);
}
