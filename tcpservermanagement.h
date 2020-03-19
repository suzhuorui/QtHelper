#ifndef TCPSERVERMANAGEMENT_H
#define TCPSERVERMANAGEMENT_H

#include <QObject>
#include "tcpserver.h"
class tcpServerManagement : public QObject
{
    Q_OBJECT
public:
    explicit tcpServerManagement(int falgcount, QObject *parent = nullptr);
    bool isHaveServer=true;
    tcpServer *server;

    bool CreateAndlistenTcp();
    void CloseAndDelListen();
    void setPort(int port);
    int getPort();
    QString getIP();
    bool isListening();
    void setZeroBit();
    int getfalgcount();



private:
    int falgcount;
    int port;
    QString ip;


signals:

public slots:

};

#endif // TCPSERVERMANAGEMENT_H
