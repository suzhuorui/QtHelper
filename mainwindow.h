#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDialog>
#include <QTimer>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "autoserialport.h"
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
#include <QRadioButton>
#include "helpdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void InitSoft();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_Tree;
    QStandardItem *m_Tree_TCPClient;
    QStandardItem *m_Tree_TCPServer;
    QStandardItem *m_Tree_UDPClient;
    QStandardItem *m_Tree_UDPServer;
    QStandardItem *m_Tree_Serial;
    autoSerialPort *autoS;

    int tcp_server_num=1;
    int tcp_client_num=1;
    int udp_server_num=1;
    int udp_client_num=1;
    int serial_num=1;

    int lastTime_Tcp_server=-1;

    QList<tcpServerManagement *> tcpServerManagementList;
    QList<tcpClient *> tcpClientList;
    QList<udpServer *> udpServerList;
    QList<udpClient *> udpClientList;
    QList<SerialPort *> serialPortList;


    QStringList findSerialPort();
    void initSerialPort();

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
};

#endif // MAINWINDOW_H
