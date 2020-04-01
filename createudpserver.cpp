#include "createudpserver.h"
#include "ui_createudpserver.h"
#include <QRegExpValidator>
#include <QMessageBox>
createUdpServer::createUdpServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createUdpServer)
{
    ui->setupUi(this);
    this->setWindowTitle("创建UDP服务器");
    connect(ui->pushButton_create,&QPushButton::clicked,this,&createUdpServer::createDevice);
    ui->lineEdit_port->setFocus();
    ui->lineEdit_port->setValidator(new QIntValidator(1024,65535));
}

createUdpServer::~createUdpServer()
{
    delete ui;
}
void createUdpServer::createDevice()
{
    //创建TCP服务器后关闭本界面
    int port=ui->lineEdit_port->text().toInt();
    if(port>=1024&&port<=65535)
    {
    emit createSignal(port);
    this->deleteLater();
    }
    else {
        QMessageBox::warning(this,"ERROR","端口号范围错误，请输入1024-65535之间的端口号",nullptr,nullptr);
    }
}
