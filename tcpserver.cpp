#include "tcpserver.h"
#include <QThread>
#include <QGraphicsOpacityEffect>
tcpServer::tcpServer(int falgcount, QObject *parent) : QTcpServer(parent)
{
    this->falgcount=falgcount;
    type=1;

}

tcpServer::~tcpServer()
{
    qDebug()<<"删除服务器";
    textEdit->deleteLater();
    textBrowser->deleteLater();
    qDebug()<<socketList.length();
    if(!socketList.isEmpty())
    {
        int i=0;
        foreach(socketRun *sock,socketList){
            qDebug()<<"第"<<++i<<"个socketRun";
            sock->deleteLater();
        }
    }
}

bool tcpServer::getState()
{
    return isListen;
}

bool tcpServer::listenTcp()
{
    bool is=this->listen(QHostAddress(myIP),quint16(myPort));
    if(is)
    {
        qDebug()<<"开始监听";
        isListen=true;
        return true;
    }
    return false;
}

void tcpServer::closeListen()
{
    this->close();
    isListen=false;
    //socketList.clear();
    textEdit->setParent(nullptr);
    textBrowser->setParent(nullptr);
    textEdit->hide();
    textBrowser->hide();
    qDebug()<<"停止监听";
    clientList.clear();
    //this->deleteLater();
}

QStringList tcpServer::getClientList()
{
    return clientList;
}

void tcpServer::sendMsg(QString str, QString addr)
{
    if(socketList.isEmpty())
        return ;
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

    if(addr=="全部客户端")
    {
        foreach(socketRun *sock,socketList){
            sock->write(data);
        }
    }
    else {
        int n=-1;
        //由于客户端IP地址List与客户端socket类同时创建所以，对应的位置相同
        //通过地址映射到socket类对象
        foreach(QString address,clientList){
            n++;
            if(address==addr)
            {
                socketList.at(n)->write(data);
                break;
            }
        }
    }
    QString str1=getBlueString("["+common::getCurrTime()+"]")+"发送"+getGreenString("[ASCII]:")+getRedString(str);
    textBrowser->append(str1);
    emit sendDateSIGNAL(sendBit,ricvBit,falgcount,type);
}

void tcpServer::sendHexMsg(QString msg, QString addr)
{
    if(socketList.isEmpty())
        return ;
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
        //QString strHex=ba.toHex().toStdString().c_str();
        QString sss=ba.toStdString().c_str();
        data=common::hexStrToByteArray(sss.toUpper());
    }
    else {
        data=common::hexStrToByteArray(msg);
    }

    if(addr=="全部客户端")
    {
        qDebug()<<"服务器->全部客户端("<<socketList.length()<<"个)";
        foreach(socketRun *sock,socketList){
            sock->write(data);
        }
    }
    else {
        int n=-1;
        //由于客户端IP地址List与客户端socket类同时创建所以，对应的位置相同
        //通过地址映射到socket类对象
        foreach(QString address,clientList){
            n++;
            if(address==addr)
            {
                socketList.at(n)->write(data);
                break;
            }
        }
    }
    QString str1=getBlueString("["+common::getCurrTime()+"]")+"发送"+getGreenString("[Hex]:")+getRedString(msg);
    //QString str1="["+common::getCurrTime()+"]发送[Hex]:"+msg;
    textBrowser->append(str1);
    emit sendDateSIGNAL(sendBit,ricvBit,falgcount,type);
}

void tcpServer::RemoveClient(int index)
{
    qDebug()<<"删除服务器中客户端信息:"<<clientList.at(index);
    clientList.removeAt(index);
}

void tcpServer::setPort(int port)
{
    myPort=port;
}

void tcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"has client connection";

    QThread *thread=new QThread();
    socketRun *socketrun=new socketRun(socketList.length(),socketDescriptor);

    QObject::connect(socketrun,&socketRun::started,socketrun,&socketRun::run);
    QObject::connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    QObject::connect(socketrun,&socketRun::readSocketSignal,this,&tcpServer::readslot,Qt::QueuedConnection);
    QObject::connect(socketrun,&socketRun::disConnectSIGNAL,this,&tcpServer::disConnSLOT);
    QObject::connect(socketrun,&socketRun::runOver,this,&tcpServer::getClientInfo);//在run函数运行结束后放入管理
    QObject::connect(socketrun,&socketRun::sendNewSendBit,this,&tcpServer::upDateSendBit);

    socketrun->moveToThread(thread);
    thread->start();
    emit socketrun->started();
}

void tcpServer::readslot(QByteArray ba,int size,QString ip)
{
    qDebug()<<"asdasd";
    QString str=QString::fromLocal8Bit(ba);
    QString strHex=ba.toHex().toStdString().c_str();
    //QString str1="["+common::getCurrTime()+"]"+ip+":["+strHex.toUpper()+"]"+str;
    QString str1=getBlueString("["+common::getCurrTime()+"]")+ip+getGreenString("：["+strHex.toUpper()+"]")+getRedString(str);
    textBrowser->append(str1);
    ricvBit+=size;
}

void tcpServer::disConnSLOT(int index)
{
    qDebug()<<"收到断开了";
    socketList.removeAt(index);
    clientList.removeAt(index);
    emit sendDisSIGNAL();
}

void tcpServer::getClientInfo(socketRun *sock)
{
    //在IP地址list中找到相同的IP地址后区分设备

    if(socketList.length()==0)
    {
        qDebug()<<"socketList.length()==0";
        socketList.append(sock);//socket放入list中管理
        clientList.append(sock->getShowIp());//找到进来客户端的ip并放入list中，传给主窗口的下拉框显示
    }
    else {
        qDebug()<<"socketList.length()=="<<socketList.length();
        int i=0;
        foreach(socketRun *s,socketList){
            if(sock->RealyIP()==s->RealyIP())
            {
                i++;
                qDebug()<<"i="<<i;
                qDebug()<<"sock->RealyIP()="<<sock->RealyIP();
                qDebug()<<"s->RealyIP()="<<s->RealyIP();

                sock->setShowIP(sock->RealyIP()+"("+QString::number(i)+")");

                qDebug()<<"sock->ShowIP="<<sock->getShowIp();
                int j=0;
                foreach(QString str,clientList)
                {
                    if(str==sock->getShowIp())
                        j++;
                }
                if(j==0)
                {
                    socketList.append(sock);//socket放入list中管理
                    clientList.append(sock->getShowIp());//找到进来客户端的ip并放入list中，传给主窗口的下拉框显示
                }
            }
        }
    }
    qDebug()<<sock->getShowIp();
    emit hasClient(sock->getShowIp(),*this);
}

void tcpServer::upDateSendBit(int size)
{
    qDebug()<<"upDateSendBit"<<size;
    sendBit+=size;
    emit sendDateSIGNAL(sendBit,ricvBit,falgcount,type);
}

void tcpServer::timerSend(QString msg, float s, int count, int sendType, QString add)
{
    qDebug()<<"点击发送";
    addr=add;
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

void tcpServer::timerSendSLOT()
{
    if(sendType==10)
    {
        if(nowCount<timerCount)
        {
            nowCount++;
            sendMsg(msg,addr);
        }
        else {
            isTimeSending=false;
            emit sendDateSIGNAL(sendBit,ricvBit,falgcount,type);
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
            sendHexMsg(msg,addr);
        }
        else {
            isTimeSending=false;
            emit sendDateSIGNAL(sendBit,ricvBit,falgcount,type);
            m_timer->stop();
            nowCount=0;
            delete m_timer;
            m_timer=nullptr;
            return;
        }
    }
}

