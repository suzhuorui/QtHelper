#include "tcpservermanagement.h"
#include <QMessageBox>
tcpServerManagement::tcpServerManagement(int falgcount,QObject *parent) : QObject(parent)
{
    this->falgcount=falgcount;
    server=new tcpServer(falgcount,this);

    QString ipaddress;
    QList<QHostAddress> list=QNetworkInterface::allAddresses();
}

bool tcpServerManagement::CreateAndlistenTcp()
{
    qDebug()<<"开始监听";
    if(!isHaveServer)
    {
        server=new tcpServer(falgcount,this);
    }
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
    return server->getMyPort();
}

QString tcpServerManagement::getIP()
{
    return server->getMyIP();
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
