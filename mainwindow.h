#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDialog>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "serialport.h"
#include "createtcpserver2.h"
#include "createtcpclient.h"
#include "createudpserver.h"
#include "createudpclient.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include "tcpservermanagement.h"
#include "udpserver.h"
#include "udpclient.h"
#include "helpdialog.h"
#include "settingdialog.h"

namespace Ui {
class MainWindow;
}

extern int DeviceNum;//当前连接数

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void InitSoft();



private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_Tree;//TreeView的模型对象
    QStandardItem *m_Tree_TCPClient;//TCP服务器的标签对象
    QStandardItem *m_Tree_TCPServer;
    QStandardItem *m_Tree_UDPClient;
    QStandardItem *m_Tree_UDPServer;
    QStandardItem *m_Tree_Serial;

    //设备名的标志位，如：TCP_Server_1
    int tcp_server_num=1;
    int tcp_client_num=1;
    int udp_server_num=1;
    int udp_client_num=1;
    int serial_num=1;

    //存放各种设备的链表
    QList<tcpServerManagement *> tcpServerManagementList;
    QList<tcpClient *> tcpClientList;
    QList<udpServer *> udpServerList;
    QList<udpClient *> udpClientList;
    QList<SerialPort *> serialPortList;


    int MaxDeviceNum=10;//最大连接数

    //查找串口设备名
    QStringList findSerialPort();
    void initSerialPort();//创建串口设备


public slots:
    void changeDeviceInfo(const QModelIndex &index);
    void createTreesun();
    void deleteTreesun();
    void open_or_close();
    void sendDate();
    void timerSendDate();
    void createTcpServer(int port);
    void createTcpClient(QString Hostip, int Hostport);
    void createUdpServer(int port);
    void createUdpClient(QString serverIp, int serverPort);
    void aboutHelp();
    void Retimer();
    void upDateDeviceInfo(int send,int ricv,int falgcount,int type);
    void updateDisInfo();
    void AsciiEditSLOT();
    void HexEditSLOT();
    void clearDataSLOT();
    void saveDataSLOT();
    void clearSendSLOT();
    void loadFileSLOT();
    void clickSetting();
    void setMaxDeviceNum(int num);
};

#endif // MAINWINDOW_H
