#include "createudpclient.h"
#include "ui_createudpclient.h"

createUdpClient::createUdpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createUdpClient)
{
    ui->setupUi(this);
    this->setWindowTitle("创建UDP客户端");
    ui->lineEdit_serverIP->setCursorPosition(0);
    connect(ui->pushButton_create,&QPushButton::clicked,this,&createUdpClient::createDevice);
    ui->lineEdit_serverIP->setFocus();
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
    if(port>0&&port<9999999)
    {
    emit createSignal(ui->lineEdit_serverIP->text(),ui->lineEdit_serverPort->text().toInt());
    this->deleteLater();
    }
}
