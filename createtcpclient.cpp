#include "createtcpclient.h"
#include "ui_createtcpclient.h"
#include <QHostAddress>
createTcpClient::createTcpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createTcpClient)
{
    ui->setupUi(this);
    this->setWindowTitle("创建TCP客户端");
    ui->lineEdit_ip->setCursorPosition(0);
    connect(ui->pushButton_create,&QPushButton::clicked,this,&createTcpClient::createDevice);
}

createTcpClient::~createTcpClient()
{
    delete ui;
}

void createTcpClient::createDevice()
{
    QStringList ip;
    ip=ui->lineEdit_ip->text().split(".");
    for(int i=0;i<4;i++)
    {
        int addr=ip[i].toInt();
        qDebug()<<addr;
        if(addr<0||addr>255)
        {
            return;
        }
    }
    int port=ui->lineEdit_port->text().toInt();
    if(port>0&&port<9999999)
    {
    emit createSignal(ui->lineEdit_ip->text(),port);
    this->deleteLater();
    }
}
