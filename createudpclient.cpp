#include "createudpclient.h"
#include "ui_createudpclient.h"
#include <QRegExpValidator>
#include <QMessageBox>
createUdpClient::createUdpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createUdpClient)
{
    ui->setupUi(this);
    this->setWindowTitle("创建UDP客户端");
    ui->lineEdit_serverIP->setCursorPosition(0);
    connect(ui->pushButton_create,&QPushButton::clicked,this,&createUdpClient::createDevice);
    ui->lineEdit_serverIP->setFocus();
    ui->lineEdit_serverPort->setValidator(new QIntValidator(1024,65535));
}

createUdpClient::~createUdpClient()
{
    delete ui;
}
void createUdpClient::createDevice()
{
    QStringList ip;
    ip=ui->lineEdit_serverIP->text().split(".");
    for(int i=0;i<4;i++)
    {
        int addr=ip[i].toInt();
        if(addr<0||addr>255)
        {
            return;
        }
    }
    int port=ui->lineEdit_serverPort->text().toInt();
    if(port>=1024&&port<=65535)
    {
    emit createSignal(ui->lineEdit_serverIP->text(),port);
    this->deleteLater();
    }
    else
        QMessageBox::warning(this,"ERROR","端口号范围错误，请输入1024-65535之间的端口号",nullptr,nullptr);
}
