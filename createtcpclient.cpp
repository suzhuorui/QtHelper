#include "createtcpclient.h"
#include "ui_createtcpclient.h"
#include <QHostAddress>
#include <QRegExpValidator>
#include <QMessageBox>
createTcpClient::createTcpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createTcpClient)
{
    ui->setupUi(this);
    this->setWindowTitle("创建TCP客户端");
    ui->lineEdit_ip->setCursorPosition(0);
    connect(ui->pushButton_create,&QPushButton::clicked,this,&createTcpClient::createDevice);
    ui->lineEdit_ip->setFocus();
    ui->lineEdit_port->setValidator(new QIntValidator(1024,65535));
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

    if(port>=1024&&port<=65535)
    {
    emit createSignal(ui->lineEdit_ip->text(),port);
    this->deleteLater();
    }
    else {
        QMessageBox::warning(this,"ERROR","端口号范围错误，请输入1024-65535之间的端口号",nullptr,nullptr);
    }
}
