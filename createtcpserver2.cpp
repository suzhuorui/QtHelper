#include "createtcpserver2.h"
#include "ui_createtcpserver2.h"

CreateTcpServer2::CreateTcpServer2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTcpServer2)
{
    ui->setupUi(this);
    this->setWindowTitle("创建TCP服务器");
    connect(ui->pushButton_create,&QPushButton::clicked,this,&CreateTcpServer2::createDevice);
}

CreateTcpServer2::~CreateTcpServer2()
{
    delete ui;
}

void CreateTcpServer2::createDevice()
{
    //创建TCP服务器后关闭本界面
    int port=ui->lineEdit_lisenPort->text().toInt();
    if(port>0&&port<9999999)
    {
        emit createSignal(ui->lineEdit_lisenPort->text().toInt());
        this->deleteLater();
    }
}
