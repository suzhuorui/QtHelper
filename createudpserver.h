#ifndef CREATEUDPSERVER_H
#define CREATEUDPSERVER_H

#include <QDialog>

namespace Ui {
class createUdpServer;
}

class createUdpServer : public QDialog
{
    Q_OBJECT

public:
    explicit createUdpServer(QWidget *parent = nullptr);
    ~createUdpServer();

private:
    Ui::createUdpServer *ui;
public slots:
    void createDevice();

signals:
    void createSignal(int port);
};

#endif // CREATEUDPSERVER_H
