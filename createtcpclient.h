#ifndef CREATETCPCLIENT_H
#define CREATETCPCLIENT_H

#include <QDialog>

namespace Ui {
class createTcpClient;
}

class createTcpClient : public QDialog
{
    Q_OBJECT

public:
    explicit createTcpClient(QWidget *parent = nullptr);
    ~createTcpClient();

private:
    Ui::createTcpClient *ui;

public slots:
    void createDevice();

signals:
    void createSignal(QString ip,int port);
};

#endif // CREATETCPCLIENT_H
