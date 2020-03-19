#ifndef CREATETCPSERVER2_H
#define CREATETCPSERVER2_H

#include <QDialog>

namespace Ui {
class CreateTcpServer2;
}

class CreateTcpServer2 : public QDialog
{
    Q_OBJECT

public:
    explicit CreateTcpServer2(QWidget *parent = nullptr);
    ~CreateTcpServer2();

private:
    Ui::CreateTcpServer2 *ui;

public slots:
    void createDevice();

signals:
    void createSignal(int port);
};

#endif // CREATETCPSERVER2_H
