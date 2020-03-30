#include "tcpservermanagement.h"
#include <QMessageBox>
tcpServerManagement::tcpServerManagement(int falgcount,QObject *parent) : QObject(parent)
{
    this->falgcount=falgcount;
    server=new tcpServer(falgcount,this);

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
    ip=ipaddress;
}

bool tcpServerManagement::CreateAndlistenTcp()
{
    qDebug()<<"开始监听";
    if(!isHaveServer)
    {
        server=new tcpServer(falgcount,this);
    }
    server->setPort(port);
    bool is=server->listenTcp();
    if(is)
    {
        isHaveServer=true;
        return true;
    }
    else {
        QMessageBox::warning(nullptr,"Error","TCP服务器监听失败，端口被占用，请删除该设备   ");
        isHaveServer=false;
        server->QTcpServer::deleteLater();
        return false;
    }
}

void tcpServerManagement::CloseAndDelListen()
{
    if(isHaveServer)
    {
        server->closeListen();
        server->QTcpServer::deleteLater();
        isHaveServer=false;
    }
}

void tcpServerManagement::setPort(int port)
{
    this->port=port;
}

int tcpServerManagement::getPort()
{
    return port;
}

QString tcpServerManagement::getIP()
{
    return ip;
}

bool tcpServerManagement::isListening()
{
    qDebug()<<"isListening";
    if(isHaveServer)
    {
        qDebug()<<"Listening";
        return server->getState();
    }
    else {
        qDebug()<<"noListening";
        return false;
    }
}

void tcpServerManagement::setZeroBit()
{
    server->setZero();
}
int tcpServerManagement::getfalgcount()
{
    return falgcount;
}
