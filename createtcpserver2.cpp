#include "createtcpserver2.h"
#include "ui_createtcpserver2.h"
#include <QRegExpValidator>
#include <QMessageBox>
CreateTcpServer2::CreateTcpServer2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTcpServer2)
{
    ui->setupUi(this);
    this->setWindowTitle("创建TCP服务器");
    connect(ui->pushButton_create,&QPushButton::clicked,this,&CreateTcpServer2::createDevice);
    ui->lineEdit_lisenPort->setFocus();
    ui->lineEdit_lisenPort->setValidator(new QIntValidator(1024,65535));
}

CreateTcpServer2::~CreateTcpServer2()
{
    delete ui;
}

void CreateTcpServer2::createDevice()
{
    //创建TCP服务器后关闭本界面
    int port=ui->lineEdit_lisenPort->text().toInt();
    if(port>=1024&&port<=65535)
    {
        emit createSignal(port);
        this->deleteLater();
    }
    else {
        QMessageBox::warning(this,"ERROR","端口号范围错误，请输入1024-65535之间的端口号",nullptr,nullptr);
    }
}
