#ifndef CREATEUDPCLIENT_H
#define CREATEUDPCLIENT_H

#include <QDialog>

namespace Ui {
class createUdpClient;
}

class createUdpClient : public QDialog
{
    Q_OBJECT

public:
    explicit createUdpClient(QWidget *parent = nullptr);
    ~createUdpClient();

private:
    Ui::createUdpClient *ui;
public slots:
    void createDevice();

signals:
    void createSignal(QString serverIp,int serverPort);
};

#endif // CREATEUDPCLIENT_H
