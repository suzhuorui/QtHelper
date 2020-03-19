#include "createudpserver.h"
#include "ui_createudpserver.h"

createUdpServer::createUdpServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createUdpServer)
{
    ui->setupUi(this);
    this->setWindowTitle("创建UDP服务器");
    connect(ui->pushButton_create,&QPushButton::clicked,this,&createUdpServer::createDevice);
}

createUdpServer::~createUdpServer()
{
    delete ui;
}
void createUdpServer::createDevice()
{
    //创建TCP服务器后关闭本界面
    int port=ui->lineEdit_port->text().toInt();
    if(port>0&&port<9999999)
    {
    emit createSignal(ui->lineEdit_port->text().toInt());
    this->deleteLater();
    }
}
