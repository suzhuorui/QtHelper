#include "settingdialog.h"
#include "ui_settingdialog.h"
#include <QRegExpValidator>
#include <QDebug>
#include <QMessageBox>
extern int DeviceNum;
SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    qDebug()<<"settingDialog DeviceNum="<<DeviceNum;
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->setValidator(new QIntValidator(1,99999));
    connect(ui->pushButton_save,&QPushButton::clicked,this,&SettingDialog::saveSLOT);
    connect(ui->radioButton_10,&QRadioButton::clicked,this,&SettingDialog::clickMoren);
    connect(ui->radioButton_renyi,&QRadioButton::clicked,this,&SettingDialog::cliekZidingyi);
    connect(ui->radioButton_wuxian,&QRadioButton::clicked,this,&SettingDialog::clickWuxianzhi);

}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::saveSLOT()
{
    if(ui->radioButton_renyi->isChecked())
    {
        num=ui->lineEdit->text().toInt();
    }
    qDebug()<<"设置num="<<num;
    if(num>=DeviceNum)
    {
        emit sendDeviceNumSIGNAL(num);
    }
    else {
        QMessageBox::warning(this,"ERROR","设置最大设备数失败（设置数小于当前设备数，请删除多余设备或重新设置）",nullptr,nullptr);
    }
    this->deleteLater();
}

void SettingDialog::cliekZidingyi()
{
    ui->lineEdit->setEnabled(true);
}

void SettingDialog::clickMoren()
{
    ui->lineEdit->clear();
    ui->lineEdit->setEnabled(false);
    num=10;
}

void SettingDialog::clickWuxianzhi()
{
    ui->lineEdit->clear();
    ui->lineEdit->setEnabled(false);
    num=9999999;
}
