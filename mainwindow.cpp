#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBrush>
#include <QGraphicsOpacityEffect>

int DeviceNum=0;
/*时间：2020.3.12
 * 基本功能：
 *      1.五种模式，tcp服务器、tcp客户端、udp服务器、udp客户端、串口。
 *      2.可以创建多设备
 *      3.时间戳显示
 *      4.ASCII/十六进制发送
 *      5.[十六进制]ASCii显示
 *      6.tcp服务器可对单个在线连接发送数据，也可勾选全部进行发送。
 *      7.整体采用单线程，tcp服务器采用多线程。
 *      8.定时器自动发送。
 *      9.清空显示区
 *      10.清空发送框
 *      11.重置计数
 *      12.导入文件数据源
 *      13.保存显示区数据到文件
*/
//点击监听后根据选择模式显示输入框TCP服务器--解决
//TCP服务器加载文件，发送按钮名字改变--解决
//检测加载文件、导出文件、清空数据框前是否存在
//添加功能，限制最大连接数，更改最大连接数。选择无限制连接或者输入最大连接数。
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/mypic/images/mFs.ico"));
    InitSoft();
    initSerialPort();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化界面
void MainWindow::InitSoft()
{
    qDebug()<<"init";
    //初始化背景
    this->setAutoFillBackground(true);
    QPalette palette=this->palette();
    QImage image(":/mypic/images/0123_4.png");
    QImage backGround=image.scaled(this->width(),this->height(),Qt::IgnoreAspectRatio);
    palette.setBrush(QPalette::Background,QBrush(backGround));
    this->setPalette(palette);

    //初始化树
    m_Tree=new QStandardItemModel(ui->treeView);

    ui->treeView->setModel(m_Tree);
    ui->treeView->expandAll();//打开所有节点
    //m_Tree->setHorizontalHeaderLabels(QStringList()<<"设备"<<"第二"<<"第三");
    QStandardItem *item=new QStandardItem("设备");
    m_Tree->setHorizontalHeaderItem(0,item);
    m_Tree_TCPClient=new QStandardItem("TCP客户端");
    //m_Tree_TCPClient->setEditable(false);//设置不可编辑
    m_Tree_TCPServer=new QStandardItem("TCP服务器");
    m_Tree_UDPClient=new QStandardItem("UDP客户端");
    m_Tree_UDPServer=new QStandardItem("UDP服务端");
    m_Tree_Serial=new QStandardItem("串口(自动检测，无需创建)");
    m_Tree->appendRow(m_Tree_TCPServer);
    m_Tree->appendRow(m_Tree_TCPClient);
    m_Tree->appendRow(m_Tree_UDPServer);
    m_Tree->appendRow(m_Tree_UDPClient);
    m_Tree->appendRow(m_Tree_Serial);

    //设置界面中的控件的透明度
    QGraphicsOpacityEffect *effect[9];
    for(int i=0;i<9;i++)
    {
        effect[i]= new QGraphicsOpacityEffect(this);
        effect[i]->setOpacity(0.85);
    }
    ui->pushButton_create->setGraphicsEffect(effect[0]);
    ui->pushButton_about->setGraphicsEffect(effect[1]);
    ui->pushButton_delete->setGraphicsEffect(effect[2]);
    ui->pushButton_sendDate->setGraphicsEffect(effect[3]);
    ui->pushButton_RestartNum->setGraphicsEffect(effect[4]);
    ui->pushButton_open_close->setGraphicsEffect(effect[5]);
    ui->toolButton_timerStartstop->setGraphicsEffect(effect[6]);
    ui->spinBox->setGraphicsEffect(effect[7]);
    ui->lineEdit_time->setGraphicsEffect(effect[8]);

    //设置TreeView为全透明
    ui->treeView->setStyleSheet("background-color:transparent");

    //初始化标签
    ui->label_Device_name->setText("无设备");
    ui->label_Device_State->setText("未连接");
    ui->label_sendNum->setText("0 字节");
    ui->label_ricvNum->setText("0 字节");

    //初始化串口下拉框
    ui->comboBox_botelv->addItem("1200");
    ui->comboBox_botelv->addItem("2400");
    ui->comboBox_botelv->addItem("4800");
    ui->comboBox_botelv->addItem("9600");
    ui->comboBox_botelv->addItem("19200");
    ui->comboBox_botelv->addItem("38400");
    ui->comboBox_botelv->addItem("57600");
    ui->comboBox_botelv->addItem("115200");
    ui->comboBox_jiou->addItem("None(无)");
    ui->comboBox_jiou->addItem("Even(奇)");
    ui->comboBox_jiou->addItem("Odd(偶)");
    ui->comboBox_jiou->addItem("Space(=0)");
    ui->comboBox_jiou->addItem("Mark(=1)");
    ui->comboBox_dateBit->addItem("5");
    ui->comboBox_dateBit->addItem("6");
    ui->comboBox_dateBit->addItem("7");
    ui->comboBox_dateBit->addItem("8");
    ui->comboBox_stopBit->addItem("1");
    ui->comboBox_stopBit->addItem("1.5");
    ui->comboBox_stopBit->addItem("2");

    QString str=QString::number(DeviceNum)+"/"+QString::number(MaxDeviceNum);
    ui->label_DeviceNum->setText(str);

    //初始化属性标签
    ui->label_set_readip->hide();
    ui->label_set_writeip->hide();
    ui->label_set_readport->hide();
    ui->label_set_writeport->hide();
    ui->comboBox_TCPclients->hide();
    ui->label_set_clients->hide();
    ui->label_set_jiou->hide();
    ui->label_set_botelv->hide();
    ui->label_set_clients->hide();
    ui->label_set_dateBit->hide();
    ui->label_set_stopBit->hide();
    ui->label_set_clienttoserverReadIP->hide();
    ui->label_set_clienttoserverWriteIP->hide();
    ui->label_set_clienttoserverReadport->hide();
    ui->label_set_clienttoserverWritePort->hide();
    ui->comboBox_jiou->hide();
    ui->comboBox_botelv->hide();
    ui->comboBox_dateBit->hide();
    ui->comboBox_stopBit->hide();

    connect(ui->treeView,&QTreeView::clicked,this,&MainWindow::changeDeviceInfo);
    connect(ui->pushButton_create,&QPushButton::clicked,this,&MainWindow::createTreesun);
    connect(ui->pushButton_delete,&QPushButton::clicked,this,&MainWindow::deleteTreesun);
    connect(ui->pushButton_open_close,&QPushButton::clicked,this,&MainWindow::open_or_close);
    connect(ui->pushButton_sendDate,&QPushButton::clicked,this,&MainWindow::sendDate);
    connect(ui->toolButton_timerStartstop,&QToolButton::clicked,this,&MainWindow::timerSendDate);
    connect(ui->pushButton_about,&QPushButton::clicked,this,&MainWindow::aboutHelp);
    connect(ui->pushButton_RestartNum,&QPushButton::clicked,this,&MainWindow::Retimer);
    connect(ui->radioButton,&QRadioButton::clicked,this,&MainWindow::AsciiEditSLOT);
    connect(ui->radioButton_2,&QRadioButton::clicked,this,&MainWindow::HexEditSLOT);
    connect(ui->pushButton_clearData,&QPushButton::clicked,this,&MainWindow::clearDataSLOT);
    connect(ui->pushButton_saveData,&QPushButton::clicked,this,&MainWindow::saveDataSLOT);
    connect(ui->pushButton_clearSend,&QPushButton::clicked,this,&MainWindow::clearSendSLOT);
    connect(ui->pushButton_loadFile,&QPushButton::clicked,this,&MainWindow::loadFileSLOT);
    connect(ui->pushButton_MaxConnectNum,&QPushButton::clicked,this,&MainWindow::clickSetting);
}

//查找串口
QStringList MainWindow::findSerialPort()
{
    QStringList strlist;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts() )
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            //ui->comboBox->addItem(info.portName());
            strlist.append(info.portName());
            serial.close();
        }
    }
    return strlist;
}

//初始化串口，创建串口对象
void MainWindow::initSerialPort()
{
    foreach(QString name,findSerialPort())
    {
        QStandardItem *newItem;
        newItem=new QStandardItem(name);
        m_Tree->item(4)->appendRow(newItem);//添加子节点
        SerialPort *newSerialPort=new SerialPort(name,serialPortList.length());
        connect(newSerialPort,&SerialPort::ricvBitSIGNAL,this,&MainWindow::upDateDeviceInfo);
        serialPortList.append(newSerialPort);
        DeviceNum++;
        QString str=QString::number(DeviceNum)+"/"+QString::number(MaxDeviceNum);
        ui->label_DeviceNum->setText(str);
    }
}

void MainWindow::setMaxDeviceNum(int num)
{
    MaxDeviceNum=num;
    QString str=QString::number(DeviceNum)+"/"+QString::number(MaxDeviceNum);
    ui->label_DeviceNum->setText(str);
}

//选择树形结构中的设备时，更改界面右上方的设备信息
void MainWindow::changeDeviceInfo(const QModelIndex &index)
{
    /*
     * 通过选择的item的preant来选择对应的QList<tcpserver/tcpclient/udp..>
     * 通过选择的item的index来在QList<>中选择设备对象
    */


    QString currName=index.data().toString();

    //通过判断当前所选中item的parent来更改相应的标签
    if(index.parent().data().toString()=="TCP服务器")
    {
        qDebug()<<"TCP服务器";

        //设置不用监听均可设置的值
        {
            ui->label_Device_name->setText(currName);
            ui->comboBox_TCPclients->clear();
            if(tcpServerManagementList.at(index.row())->server->isTimerSending())
            {
                ui->toolButton_timerStartstop->setText("停止发送");
            }
            else {
                ui->toolButton_timerStartstop->setText("定时发送");
            }
            //隐藏其他类型的输入输出框
            foreach(tcpClient *cli,tcpClientList){
                cli->textEdit->setParent(nullptr);
                cli->textBrowser->setParent(nullptr);
                cli->textEdit->hide();
                cli->textBrowser->hide();
                cli->hexEdit->setParent(nullptr);
                cli->hexEdit->hide();
            }
            foreach(udpServer *ser,udpServerList){
                ser->textEdit->setParent(nullptr);
                ser->textBrowser->setParent(nullptr);
                ser->textEdit->hide();
                ser->textBrowser->hide();
                ser->hexEdit->setParent(nullptr);
                ser->hexEdit->hide();
            }
            foreach(udpClient *cli,udpClientList){
                cli->textEdit->setParent(nullptr);
                cli->textBrowser->setParent(nullptr);
                cli->textEdit->hide();
                cli->textBrowser->hide();
                cli->hexEdit->setParent(nullptr);
                cli->hexEdit->hide();
            }
            foreach(SerialPort *port,serialPortList){
                port->textEdit->setParent(nullptr);
                port->textBrowser->setParent(nullptr);
                port->textEdit->hide();
                port->textBrowser->hide();
                port->hexEdit->setParent(nullptr);
                port->hexEdit->hide();
            }


            ui->pushButton_open_close->show();
            ui->pushButton_loadFile->setText("加载文件");
            ui->label_set_readip->setText("本机IP:");
            ui->label_set_writeip->setText(tcpServerManagementList.at(index.row())->getIP());
            ui->label_set_readport->setText("监听端口:");
            qDebug()<<QString::number(tcpServerManagementList.at(index.row())->getPort());
            ui->label_set_writeport->setText(QString::number(tcpServerManagementList.at(index.row())->getPort()));
            ui->label_set_clients->setText("发送到:");

            ui->label_set_readip->show();
            ui->label_set_writeip->show();
            ui->label_set_readport->show();
            ui->label_set_writeport->show();
            ui->comboBox_TCPclients->show();
            ui->label_set_clients->show();

            ui->label_set_jiou->hide();
            ui->label_set_botelv->hide();
            ui->label_set_dateBit->hide();
            ui->label_set_stopBit->hide();
            ui->label_set_clienttoserverReadIP->hide();
            ui->label_set_clienttoserverWriteIP->hide();
            ui->label_set_clienttoserverReadport->hide();
            ui->label_set_clienttoserverWritePort->hide();
            ui->comboBox_jiou->hide();
            ui->comboBox_botelv->hide();
            ui->comboBox_dateBit->hide();
            ui->comboBox_stopBit->hide();


        }

        //设置  监听开始后的值
        if(tcpServerManagementList.at(index.row())->isListening())
        {
            if(ui->radioButton->isChecked())
            {
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->hide();
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->setParent(nullptr);
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textEdit->setParent(ui->groupBox_send);
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textEdit->show();
                qDebug()<<"TcpServer字符发送";
            }
            else {
                qDebug()<<"TcpServer16进制发送";
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textEdit->hide();
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textEdit->setParent(nullptr);
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->setParent(ui->groupBox_send);
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->show();
            }
            if(tcpServerManagementList.at(index.row())->server->isFile)
            {
                ui->pushButton_loadFile->setText("停止加载");
            }
            else {
                ui->pushButton_loadFile->setText("加载文件");
            }

            qDebug()<<"正在监听";
            ui->pushButton_open_close->setText("停止监听");
            ui->label_Device_State->setText("就绪");
            //判断客户端是否为0,把客户端加入combobox中
            ui->comboBox_TCPclients->addItem("全部客户端");
            if(!tcpServerManagementList.at(index.row())->server->getClientList().isEmpty())
            {
                foreach(QString client,tcpServerManagementList.at(index.row())->server->getClientList()){
                    ui->comboBox_TCPclients->addItem(client);
                }
            }

            ui->label_sendNum->setText(QString::number(tcpServerManagementList.at(index.row())->server->getSendBit())+" 字节");
            ui->label_ricvNum->setText(QString::number(tcpServerManagementList.at(index.row())->server->getRicvBit())+" 字节");
            tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textBrowser->setParent(ui->groupBox_read);
            tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textBrowser->show();
            //显示所选中的服务器的输入输出框，其他的都隐藏
            //隐藏其他服务器的输入输出框
            int i=-1;
            foreach(tcpServerManagement *serM,tcpServerManagementList){
                if(++i==index.row())
                    continue;
                else {
                    if(serM->isHaveServer)
                    {
                        serM->server->textEdit->setParent(nullptr);
                        serM->server->textBrowser->setParent(nullptr);
                        serM->server->hexEdit->setParent(nullptr);
                        serM->server->hexEdit->hide();
                        serM->server->textEdit->hide();
                        serM->server->textBrowser->hide();
                    }
                }
            }
        }
        else {
            ui->pushButton_open_close->setText("开始监听");
            ui->label_Device_State->setText("未就绪");
            qDebug()<<"没有监听";
        }
    }
    else if (index.parent().data().toString()=="TCP客户端") {
        tcpClient *client=tcpClientList.at(index.row());
        //设置不用连接均可设置的值
        {
            ui->label_Device_name->setText(currName);
            if(client->isTimerSending())
            {
                ui->toolButton_timerStartstop->setText("停止发送");
            }
            else {
                ui->toolButton_timerStartstop->setText("定时发送");
            }

            ui->pushButton_open_close->show();
            ui->pushButton_loadFile->setText("加载文件");
            ui->label_set_readip->setText("本机IP:");
            ui->label_set_writeip->setText(tcpClientList.at(index.row())->getMyIP());
            ui->label_set_clienttoserverReadIP->setText("服务器IP:");
            ui->label_set_clienttoserverWriteIP->setText(client->getOtherIP());
            ui->label_set_clienttoserverReadport->setText("服务器端口:");
            ui->label_set_clienttoserverWritePort->setText(QString::number(client->getOtherPort()));

            ui->label_sendNum->setText(QString::number(client->getSendBit())+" 字节");
            ui->label_ricvNum->setText(QString::number(client->getRicvBit())+" 字节");


            //隐藏其他客户端的输入输出框
            int i=-1;
            foreach(tcpClient *cli,tcpClientList){
                if(++i==index.row())
                    continue;
                cli->textEdit->setParent(nullptr);
                cli->textBrowser->setParent(nullptr);
                cli->textEdit->hide();
                cli->textBrowser->hide();
                cli->hexEdit->setParent(nullptr);
                cli->hexEdit->hide();
            }
            //隐藏其他类型的输入输出框
            foreach(tcpServerManagement *serM,tcpServerManagementList){
                if(serM->isHaveServer)
                {
                    serM->server->textEdit->setParent(nullptr);
                    serM->server->textBrowser->setParent(nullptr);
                    serM->server->textEdit->hide();
                    serM->server->textBrowser->hide();
                    serM->server->hexEdit->setParent(nullptr);
                    serM->server->hexEdit->hide();
                }
            }
            foreach(udpServer *ser,udpServerList){
                ser->textEdit->setParent(nullptr);
                ser->textBrowser->setParent(nullptr);
                ser->textEdit->hide();
                ser->textBrowser->hide();
                ser->hexEdit->setParent(nullptr);
                ser->hexEdit->hide();
            }
            foreach(udpClient *cli,udpClientList){
                cli->textEdit->setParent(nullptr);
                cli->textBrowser->setParent(nullptr);
                cli->textEdit->hide();
                cli->textBrowser->hide();
                cli->hexEdit->setParent(nullptr);
                cli->hexEdit->hide();
            }
            foreach(SerialPort *port,serialPortList){
                port->textEdit->setParent(nullptr);
                port->textBrowser->setParent(nullptr);
                port->textEdit->hide();
                port->textBrowser->hide();
                port->hexEdit->setParent(nullptr);
                port->hexEdit->hide();
            }

            ui->label_set_readip->show();
            ui->label_set_writeip->show();
            ui->label_set_readport->show();
            ui->label_set_writeport->show();
            ui->label_set_clienttoserverReadIP->show();
            ui->label_set_clienttoserverWriteIP->show();
            ui->label_set_clienttoserverReadport->show();
            ui->label_set_clienttoserverWritePort->show();

            ui->label_set_jiou->hide();
            ui->label_set_botelv->hide();
            ui->label_set_clients->hide();
            ui->label_set_dateBit->hide();
            ui->label_set_stopBit->hide();
            ui->label_set_readport->hide();
            ui->label_set_writeport->hide();
            ui->comboBox_TCPclients->hide();
            ui->comboBox_jiou->hide();
            ui->comboBox_botelv->hide();
            ui->comboBox_dateBit->hide();
            ui->comboBox_stopBit->hide();
        }

        if(client->isConnection)
        {
            if(tcpClientList.at(index.row())->isFile)
            {
                ui->pushButton_loadFile->setText("停止加载");
            }
            else {
                ui->pushButton_loadFile->setText("加载文件");
            }

            //显示所选中的服务器的输入输出框，其他的都隐藏
            if(ui->radioButton->isChecked())
            {
                qDebug()<<"TcpClient字符发送";
                tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->hide();
                tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(nullptr);
                tcpClientList.at(ui->treeView->currentIndex().row())->textEdit->setParent(ui->groupBox_send);
                tcpClientList.at(ui->treeView->currentIndex().row())->textEdit->show();
            }
            else
            {
                qDebug()<<"TcpClient16进制发送";
                tcpClientList.at(ui->treeView->currentIndex().row())->textEdit->hide();
                tcpClientList.at(ui->treeView->currentIndex().row())->textEdit->setParent(nullptr);
                tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(ui->groupBox_send);
                tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->show();
            }
            ui->pushButton_open_close->setText("断开连接");
            ui->label_Device_State->setText("就绪");
            tcpClientList.at(ui->treeView->currentIndex().row())->textBrowser->setParent(ui->groupBox_read);
            tcpClientList.at(ui->treeView->currentIndex().row())->textBrowser->show();

        }
        else {
            ui->pushButton_open_close->setText("连接");
            ui->label_Device_State->setText("未就绪");
        }

    }
    else if (index.parent().data().toString()=="UDP服务端") {
        qDebug()<<index.row();
        ui->label_Device_name->setText(currName);
        ui->pushButton_open_close->hide();

        if(udpServerList.at(index.row())->isFile)
        {
            ui->pushButton_loadFile->setText("停止加载");
        }
        else {
            ui->pushButton_loadFile->setText("加载文件");
        }

        if(udpServerList.at(index.row())->isTimerSending())
        {
            ui->toolButton_timerStartstop->setText("停止发送");
        }
        else {
            ui->toolButton_timerStartstop->setText("定时发送");
        }
        if(ui->radioButton->isChecked())
        {
            udpServerList.at(ui->treeView->currentIndex().row())->hexEdit->hide();
            udpServerList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(nullptr);
            udpServerList.at(ui->treeView->currentIndex().row())->textEdit->setParent(ui->groupBox_send);
            udpServerList.at(ui->treeView->currentIndex().row())->textEdit->show();
        }
        else {
            udpServerList.at(ui->treeView->currentIndex().row())->textEdit->hide();
            udpServerList.at(ui->treeView->currentIndex().row())->textEdit->setParent(nullptr);
            udpServerList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(ui->groupBox_send);
            udpServerList.at(ui->treeView->currentIndex().row())->hexEdit->show();
        }
        udpServerList.at(ui->treeView->currentIndex().row())->textBrowser->setParent(ui->groupBox_read);
        udpServerList.at(ui->treeView->currentIndex().row())->textBrowser->show();
        ui->label_Device_State->setText("就绪");
        ui->label_set_readip->setText("本机IP:");
        ui->label_set_writeip->setText(udpServerList.at(ui->treeView->currentIndex().row())->getMyIP());
        ui->label_set_readport->setText("本机端口:");
        ui->label_set_writeport->setText(QString::number(udpServerList.at(ui->treeView->currentIndex().row())->getMyPort()));

        //ui->label_set_clients->setText("对方IP:");
        ui->label_set_clienttoserverReadIP->setText("对方IP：");
        ui->label_set_clienttoserverWriteIP->setText(udpServerList.at(ui->treeView->currentIndex().row())->getOtherIP());
        ui->label_set_clienttoserverReadport->setText("对方端口：");
        ui->label_set_clienttoserverWritePort->setText(QString::number(udpServerList.at(ui->treeView->currentIndex().row())->getOtherPort()));

        ui->label_sendNum->setText(QString::number(udpServerList.at(ui->treeView->currentIndex().row())->getSendBit())+" 字节");
        ui->label_ricvNum->setText(QString::number(udpServerList.at(ui->treeView->currentIndex().row())->getRicvBit())+" 字节");

        int i=-1;
        foreach(udpServer *ser,udpServerList){
            if(++i==index.row())
                continue;
            ser->textEdit->setParent(nullptr);
            ser->textBrowser->setParent(nullptr);
            ser->textEdit->hide();
            ser->textBrowser->hide();
            ser->hexEdit->setParent(nullptr);
            ser->hexEdit->hide();
        }

        //隐藏其他类型的输入输出框
        foreach(tcpClient *cli,tcpClientList){
            cli->textEdit->setParent(nullptr);
            cli->textBrowser->setParent(nullptr);
            cli->textEdit->hide();
            cli->textBrowser->hide();
            cli->hexEdit->setParent(nullptr);
            cli->hexEdit->hide();
        }
        foreach(tcpServerManagement *serM,tcpServerManagementList){
            if(serM->isHaveServer)
            {
                serM->server->textEdit->setParent(nullptr);
                serM->server->textBrowser->setParent(nullptr);
                serM->server->textEdit->hide();
                serM->server->textBrowser->hide();
                serM->server->hexEdit->setParent(nullptr);
                serM->server->hexEdit->hide();
            }
        }
        foreach(udpClient *cli,udpClientList){
            cli->textEdit->setParent(nullptr);
            cli->textBrowser->setParent(nullptr);
            cli->textEdit->hide();
            cli->textBrowser->hide();
            cli->hexEdit->setParent(nullptr);
            cli->hexEdit->hide();
        }
        foreach(SerialPort *port,serialPortList){
            port->textEdit->setParent(nullptr);
            port->textBrowser->setParent(nullptr);
            port->textEdit->hide();
            port->textBrowser->hide();
            port->hexEdit->setParent(nullptr);
            port->hexEdit->hide();
        }

        ui->label_set_clienttoserverReadIP->show();
        ui->label_set_clienttoserverWriteIP->show();
        ui->label_set_clienttoserverReadport->show();
        ui->label_set_clienttoserverWritePort->show();

        ui->label_set_readip->show();
        ui->label_set_writeip->show();
        ui->label_set_readport->show();
        ui->label_set_writeport->show();

        ui->label_set_clients->hide();
        ui->label_set_jiou->hide();
        ui->label_set_botelv->hide();
        ui->label_set_dateBit->hide();
        ui->label_set_stopBit->hide();
        ui->comboBox_TCPclients->hide();
    }
    else if (index.parent().data().toString()=="UDP客户端") {
        qDebug()<<index.row();

        if(udpClientList.at(index.row())->isFile)
        {
            ui->pushButton_loadFile->setText("停止加载");
        }
        else {
            ui->pushButton_loadFile->setText("加载文件");
        }

        if(udpClientList.at(index.row())->isTimerSending())
        {
            ui->toolButton_timerStartstop->setText("停止发送");
        }
        else {
            ui->toolButton_timerStartstop->setText("定时发送");
        }

        if(ui->radioButton->isChecked())
        {
            udpClientList.at(ui->treeView->currentIndex().row())->hexEdit->hide();
            udpClientList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(nullptr);
            udpClientList.at(ui->treeView->currentIndex().row())->textEdit->setParent(ui->groupBox_send);
            udpClientList.at(ui->treeView->currentIndex().row())->textEdit->show();
        }
        else
        {
            udpClientList.at(ui->treeView->currentIndex().row())->textEdit->hide();
            udpClientList.at(ui->treeView->currentIndex().row())->textEdit->setParent(nullptr);
            udpClientList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(ui->groupBox_send);
            udpClientList.at(ui->treeView->currentIndex().row())->hexEdit->show();
        }

        ui->label_Device_name->setText(currName);
        ui->pushButton_open_close->hide();
        ui->label_Device_State->setText("就绪");
        ui->label_set_readip->setText("本机IP:");
        ui->label_set_writeip->setText(udpClientList.at(index.row())->getMyIP());
        ui->label_set_clienttoserverReadIP->setText("对方IP:");
        ui->label_set_clienttoserverWriteIP->setText(udpClientList.at(index.row())->getOtherIP());
        ui->label_set_clienttoserverReadport->setText("对方端口:");
        ui->label_set_clienttoserverWritePort->setText(QString::number(udpClientList.at(index.row())->getOtherPort()));

        ui->label_sendNum->setText(QString::number(udpClientList.at(ui->treeView->currentIndex().row())->getSendBit())+" 字节");
        ui->label_ricvNum->setText(QString::number(udpClientList.at(ui->treeView->currentIndex().row())->getRicvBit())+" 字节");

        udpClientList.at(ui->treeView->currentIndex().row())->textBrowser->setParent(ui->groupBox_read);
        udpClientList.at(ui->treeView->currentIndex().row())->textBrowser->show();

        //隐藏除自己外的其他udp客户端
        int i=-1;
        foreach(udpClient *cli,udpClientList){
            if(++i==index.row())
                continue;
            cli->textEdit->setParent(nullptr);
            cli->textBrowser->setParent(nullptr);
            cli->textEdit->hide();
            cli->textBrowser->hide();
            cli->hexEdit->setParent(nullptr);
            cli->hexEdit->hide();
        }

        //隐藏其他类型的输入输出框
        //隐藏tcp客户端
        foreach(tcpClient *cli,tcpClientList){
            cli->textEdit->setParent(nullptr);
            cli->textBrowser->setParent(nullptr);
            cli->textEdit->hide();
            cli->textBrowser->hide();
            cli->hexEdit->setParent(nullptr);
            cli->hexEdit->hide();
        }
        //隐藏tcp服务器
        foreach(tcpServerManagement *serM,tcpServerManagementList){
            if(serM->isHaveServer)
            {
                serM->server->textEdit->setParent(nullptr);
                serM->server->textBrowser->setParent(nullptr);
                serM->server->textEdit->hide();
                serM->server->textBrowser->hide();
                serM->server->hexEdit->setParent(nullptr);
                serM->server->hexEdit->hide();
            }
        }
        //隐藏udp服务器
        foreach(udpServer *serM,udpServerList){
                serM->textEdit->setParent(nullptr);
                serM->textBrowser->setParent(nullptr);
                serM->textEdit->hide();
                serM->textBrowser->hide();
                serM->hexEdit->setParent(nullptr);
                serM->hexEdit->hide();
        }
        foreach(SerialPort *port,serialPortList){
            port->textEdit->setParent(nullptr);
            port->textBrowser->setParent(nullptr);
            port->textEdit->hide();
            port->textBrowser->hide();
            port->hexEdit->setParent(nullptr);
            port->hexEdit->hide();
        }

        ui->label_set_readip->show();
        ui->label_set_writeip->show();
        ui->label_set_readport->show();
        ui->label_set_writeport->show();
        ui->label_set_clienttoserverReadIP->show();
        ui->label_set_clienttoserverWriteIP->show();
        ui->label_set_clienttoserverReadport->show();
        ui->label_set_clienttoserverWritePort->show();

        ui->label_set_jiou->hide();
        ui->label_set_botelv->hide();
        ui->label_set_clients->hide();
        ui->label_set_dateBit->hide();
        ui->label_set_stopBit->hide();
        ui->label_set_readport->hide();
        ui->label_set_writeport->hide();
        ui->comboBox_TCPclients->hide();
        ui->comboBox_jiou->hide();
        ui->comboBox_botelv->hide();
        ui->comboBox_dateBit->hide();
        ui->comboBox_stopBit->hide();
    }
    else if (index.parent().data().toString()=="串口(自动检测，无需创建)") {
        qDebug()<<index.row();

        if(serialPortList.at(index.row())->isFile)
        {
            ui->pushButton_loadFile->setText("停止加载");
        }
        else {
            ui->pushButton_loadFile->setText("加载文件");
        }

        if(serialPortList.at(index.row())->isTimerSending())
        {
            ui->toolButton_timerStartstop->setText("停止发送");
        }
        else {
            ui->toolButton_timerStartstop->setText("定时发送");
        }

        if(ui->radioButton->isChecked())
        {
            serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->hide();
            serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(nullptr);
            serialPortList.at(ui->treeView->currentIndex().row())->textEdit->setParent(ui->groupBox_send);
            serialPortList.at(ui->treeView->currentIndex().row())->textEdit->show();
        }
        else {
            serialPortList.at(ui->treeView->currentIndex().row())->textEdit->hide();
            serialPortList.at(ui->treeView->currentIndex().row())->textEdit->setParent(nullptr);
            serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(ui->groupBox_send);
            serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->show();
        }

        //隐藏tcp客户端
        foreach(tcpClient *cli,tcpClientList){
            cli->textEdit->setParent(nullptr);
            cli->textBrowser->setParent(nullptr);
            cli->textEdit->hide();
            cli->textBrowser->hide();
        }
        //隐藏tcp服务器
        foreach(tcpServerManagement *serM,tcpServerManagementList){
            if(serM->isHaveServer)
            {
                serM->server->textEdit->setParent(nullptr);
                serM->server->textBrowser->setParent(nullptr);
                serM->server->textEdit->hide();
                serM->server->textBrowser->hide();
            }
        }
        //隐藏udp服务器
        foreach(udpServer *serM,udpServerList){
                serM->textEdit->setParent(nullptr);
                serM->textBrowser->setParent(nullptr);
                serM->textEdit->hide();
                serM->textBrowser->hide();
        }
        //隐藏udp客户端
        foreach(udpClient *cli,udpClientList){
            cli->textEdit->setParent(nullptr);
            cli->textBrowser->setParent(nullptr);
            cli->textEdit->hide();
            cli->textBrowser->hide();
        }
        //隐藏别人
        int i=-1;
        foreach(SerialPort *port,serialPortList){
            if(++i==index.row())
                continue;
            port->textEdit->setParent(nullptr);
            port->textBrowser->setParent(nullptr);
            port->textEdit->hide();
            port->textBrowser->hide();
        }
        ui->label_Device_name->setText(currName);
        if(serialPortList.at(index.row())->getState())
        {
            ui->pushButton_open_close->setText("关闭串口");
            ui->label_Device_State->setText("就绪");

            //显示自己
            serialPortList.at(ui->treeView->currentIndex().row())->textBrowser->setParent(ui->groupBox_read);
            serialPortList.at(ui->treeView->currentIndex().row())->textBrowser->show();
        }
        else {
            ui->pushButton_open_close->setText("打开串口");
            ui->label_Device_State->setText("未就绪");
        }

        ui->pushButton_open_close->show();

        ui->label_set_botelv->setText("波特率:");
        ui->label_set_stopBit->setText("停止位:");
        ui->label_set_jiou->setText("奇偶校验:");
        ui->label_set_dateBit->setText("数据位:");

        ui->comboBox_botelv->setCurrentIndex(serialPortList.at(index.row())->strBaudRate);
        ui->comboBox_dateBit->setCurrentIndex(serialPortList.at(index.row())->strDataBits);
        ui->comboBox_jiou->setCurrentIndex(serialPortList.at(index.row())->strParity);
        ui->comboBox_stopBit->setCurrentIndex(serialPortList.at(index.row())->strStopBits);





        ui->label_set_botelv->show();
        ui->label_set_stopBit->show();
        ui->label_set_jiou->show();
        ui->label_set_dateBit->show();
        ui->comboBox_jiou->show();
        ui->comboBox_botelv->show();
        ui->comboBox_dateBit->show();
        ui->comboBox_stopBit->show();

        ui->label_set_clients->hide();
        ui->label_set_readport->hide();
        ui->label_set_writeport->hide();
        ui->comboBox_TCPclients->hide();
        ui->label_set_readip->hide();
        ui->label_set_writeip->hide();
        ui->label_set_clienttoserverReadIP->hide();
        ui->label_set_clienttoserverWriteIP->hide();
        ui->label_set_clienttoserverReadport->hide();
        ui->label_set_clienttoserverWritePort->hide();
    }
    else {
        //选中第一级节点
        qDebug()<<"选中第一级节点";
        ui->label_Device_name->setText("无设备");
        ui->label_Device_State->setText("未连接");
        ui->pushButton_open_close->setText("连接");
        ui->pushButton_loadFile->setText("加载文件");
        foreach(SerialPort *port,serialPortList){
            port->textEdit->setParent(nullptr);
            port->textBrowser->setParent(nullptr);
            port->textEdit->hide();
            port->textBrowser->hide();
            port->hexEdit->setParent(nullptr);
            port->hexEdit->hide();
        }
        foreach(tcpServerManagement *serM,tcpServerManagementList){
            qDebug()<<"isHaveServer="<<serM->isHaveServer;
            if(serM->isHaveServer)
            {
                serM->server->textEdit->setParent(nullptr);
                serM->server->textBrowser->setParent(nullptr);
                serM->server->textEdit->hide();
                serM->server->textBrowser->hide();
                serM->server->hexEdit->setParent(nullptr);
                serM->server->hexEdit->hide();
            }
        }
        foreach(tcpClient *cl,tcpClientList){
            cl->textEdit->setParent(nullptr);
            cl->textEdit->hide();
            cl->textBrowser->setParent(nullptr);
            cl->textBrowser->hide();
            cl->hexEdit->setParent(nullptr);
            cl->hexEdit->hide();
        }
        foreach(udpServer *ser,udpServerList){
            ser->textEdit->setParent(nullptr);
            ser->textBrowser->setParent(nullptr);
            ser->textEdit->hide();
            ser->textBrowser->hide();
            ser->hexEdit->setParent(nullptr);
            ser->hexEdit->hide();
        }
        foreach(udpClient *cli,udpClientList){
            cli->textEdit->setParent(nullptr);
            cli->textBrowser->setParent(nullptr);
            cli->textEdit->hide();
            cli->textBrowser->hide();
            cli->hexEdit->setParent(nullptr);
            cli->hexEdit->hide();
        }


        ui->label_set_readip->hide();
        ui->label_set_writeip->hide();
        ui->label_set_readport->hide();
        ui->label_set_writeport->hide();
        ui->comboBox_TCPclients->hide();
        ui->label_set_clients->hide();
        ui->label_set_jiou->hide();
        ui->label_set_botelv->hide();
        ui->label_set_clients->hide();
        ui->label_set_dateBit->hide();
        ui->label_set_stopBit->hide();
        ui->label_set_clienttoserverReadIP->hide();
        ui->label_set_clienttoserverWriteIP->hide();
        ui->label_set_clienttoserverReadport->hide();
        ui->label_set_clienttoserverWritePort->hide();
        ui->comboBox_jiou->hide();
        ui->comboBox_botelv->hide();
        ui->comboBox_dateBit->hide();
        ui->comboBox_stopBit->hide();

    }
}

//创建子树
void MainWindow::createTreesun()
{
    if(DeviceNum>=MaxDeviceNum)
    {
        QMessageBox::warning(this,"ERROR","超过最大设备数，创建失败!",nullptr,nullptr);
        return;
    }
    if(ui->treeView->currentIndex().data().toString()=="TCP服务器")
    {
        class CreateTcpServer2 *dialog=new class CreateTcpServer2();
        dialog->show();
        connect(dialog,&CreateTcpServer2::createSignal,this,&MainWindow::createTcpServer);
    }
    else if (ui->treeView->currentIndex().data().toString()=="TCP客户端") {
        class createTcpClient *dialog=new class createTcpClient();
        dialog->show();
        connect(dialog,&createTcpClient::createSignal,this,&MainWindow::createTcpClient);
    }
    else if (ui->treeView->currentIndex().data().toString()=="UDP服务端") {
        class createUdpServer *dialog=new class createUdpServer();
        dialog->show();
        connect(dialog,&createUdpServer::createSignal,this,&MainWindow::createUdpServer);
    }
    else if (ui->treeView->currentIndex().data().toString()=="UDP客户端") {
        class createUdpClient *dialog=new class createUdpClient();
        dialog->show();
        connect(dialog,&createUdpClient::createSignal,this,&MainWindow::createUdpClient);
    }
}

//删除子树
void MainWindow::deleteTreesun()
{
    //判断所选item的父亲来确定类型
    qDebug()<<"点击删除";
    if(ui->treeView->currentIndex().parent().data().toString()=="TCP服务器")
    {
        qDebug()<<"点击删除TCP服务器";
        tcpServerManagementList.at(ui->treeView->currentIndex().row())->CloseAndDelListen();
        delete tcpServerManagementList.at(ui->treeView->currentIndex().row());
        tcpServerManagementList.removeAt(ui->treeView->currentIndex().row());
        m_Tree->removeRow(ui->treeView->currentIndex().row(),ui->treeView->currentIndex().parent());
        DeviceNum--;
        //*最后在移除item，如果先移除那么当前所选中的就变了
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="TCP客户端") {
        //在删除客户端时，要把连接服务器的客户端从conbobox中删掉,要把服务器中保存客户端信息的list中的数据删掉
        qDebug()<<"点击删除TCP客户端";
        ui->comboBox_TCPclients->removeItem(ui->treeView->currentIndex().row()+1);
        tcpClientList.at(ui->treeView->currentIndex().row())->disConnectToHost();
        delete tcpClientList.at(ui->treeView->currentIndex().row());
        tcpClientList.removeAt(ui->treeView->currentIndex().row());
        m_Tree->removeRow(ui->treeView->currentIndex().row(),ui->treeView->currentIndex().parent());
        DeviceNum--;
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP服务端") {
        qDebug()<<"点击删除UDP服务器";
        delete udpServerList.at(ui->treeView->currentIndex().row());
        udpServerList.removeAt(ui->treeView->currentIndex().row());
        m_Tree->removeRow(ui->treeView->currentIndex().row(),ui->treeView->currentIndex().parent());
        DeviceNum--;
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP客户端") {
        qDebug()<<"点击删除UDP客户端";
        delete udpClientList.at(ui->treeView->currentIndex().row());
        udpClientList.removeAt(ui->treeView->currentIndex().row());
        m_Tree->removeRow(ui->treeView->currentIndex().row(),ui->treeView->currentIndex().parent());
        DeviceNum--;
    }
    QString str=QString::number(DeviceNum)+"/"+QString::number(MaxDeviceNum);
    ui->label_DeviceNum->setText(str);
}

//打开关闭设备,UDP无需连接
void MainWindow::open_or_close()
{
    //打开连接前或者关闭连接前先判断所选中的树是哪个节点
    QString currName=ui->treeView->currentIndex().data().toString();
    if(currName=="TCP服务器"||currName=="TCP客户端"||currName=="UDP服务端"||currName=="UDP客户端"||currName=="串口(自动检测，无需创建)"){
        //不作操作
    }
    else if(ui->treeView->currentIndex().parent().data().toString()=="TCP客户端"){
        if(tcpClientList.at(ui->treeView->currentIndex().row())->isConnection)
        {
            tcpClientList.at(ui->treeView->currentIndex().row())->disConnectToHost();
            ui->label_Device_State->setText("未就绪");
            ui->pushButton_open_close->setText("连接");
        }
        else {
            if(tcpClientList.at(ui->treeView->currentIndex().row())->connectToHost())
            {
                qDebug()<<"点击客户端连接";
                ui->pushButton_open_close->setText("断开连接");
                ui->label_Device_State->setText("就绪");
                if(ui->radioButton->isChecked())
                {
                    tcpClientList.at(ui->treeView->currentIndex().row())->textEdit->setParent(ui->groupBox_send);
                    tcpClientList.at(ui->treeView->currentIndex().row())->textEdit->show();
                }
                else {
                    tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(ui->groupBox_send);
                    tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->show();
                }
                tcpClientList.at(ui->treeView->currentIndex().row())->textBrowser->setParent(ui->groupBox_read);
                tcpClientList.at(ui->treeView->currentIndex().row())->textBrowser->show();
            }
            else {
                QMessageBox::warning(this,"错误","连接服务器失败，请检查服务器IP地址与端口",nullptr,nullptr);
            }
        }
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="串口(自动检测，无需创建)") {
        if(!serialPortList.at(ui->treeView->currentIndex().row())->getState())
        {
            if(serialPortList.at(ui->treeView->currentIndex().row())->openSerialPort(
                        ui->comboBox_botelv->currentIndex(),ui->comboBox_dateBit->currentIndex(),
                        ui->comboBox_jiou->currentIndex(),ui->comboBox_stopBit->currentIndex()))
            {
                ui->pushButton_open_close->setText("关闭串口");
                ui->label_Device_State->setText("就绪");
                if(ui->radioButton->isChecked())
                {
                    serialPortList.at(ui->treeView->currentIndex().row())->textEdit->setParent(ui->groupBox_send);
                    serialPortList.at(ui->treeView->currentIndex().row())->textEdit->show();
                }
                else {
                    serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(ui->groupBox_send);
                    serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->show();
                }
                serialPortList.at(ui->treeView->currentIndex().row())->textBrowser->setParent(ui->groupBox_read);
                serialPortList.at(ui->treeView->currentIndex().row())->textBrowser->show();
            }
            else {
                QMessageBox::warning(this,"错误","打开串口失败，确保串口存在，或请重启软件",nullptr,nullptr);
            }
        }
        else {
            serialPortList.at(ui->treeView->currentIndex().row())->closeSerialPort();
            ui->pushButton_open_close->setText("打开串口");
            ui->label_Device_State->setText("未就绪");
        }

    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="TCP服务器") {
        if(!tcpServerManagementList.at(ui->treeView->currentIndex().row())->isListening())
        {
            qDebug()<<"点击监听";
            if(tcpServerManagementList.at(ui->treeView->currentIndex().row())->CreateAndlistenTcp())
            {
                ui->comboBox_TCPclients->addItem("全部客户端");
                ui->pushButton_open_close->setText("停止监听");
                ui->label_Device_State->setText("就绪");
                if(ui->radioButton->isChecked())
                {
                    tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textEdit->setParent(ui->groupBox_send);
                    tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textEdit->show();
                }
                else {
                    tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->setParent(ui->groupBox_send);
                    tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->show();
                }
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textBrowser->setParent(ui->groupBox_read);
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textBrowser->show();
            }
            else {
                QMessageBox::warning(this,"错误","监听失败，请确保端口正确且没有被占用",nullptr,nullptr);
            }
        }
        else
        {
            qDebug()<<"点击停止监听";
            tcpServerManagementList.at(ui->treeView->currentIndex().row())->CloseAndDelListen();
            ui->comboBox_TCPclients->clear();
            ui->pushButton_open_close->setText("开始监听");
            ui->label_Device_State->setText("未就绪");
        }
    }
}

//发送数据
void MainWindow::sendDate()
{
    /*首先判断是哪个设备，在进行发送
     *通过选择的item的preant来选择对应的QList<tcpserver/tcpclient/udp..>
     * 通过选择的item的index来在QList<>中选择设备对象*/
    int currIndex=ui->treeView->currentIndex().row();
    QString parentName=ui->treeView->currentIndex().parent().data().toString();

    //通过判断当前所选中item的parent来更改相应的标签
    if(parentName=="TCP服务器")
    {   
        if(tcpServerManagementList.at(currIndex)->isListening())
        {
            qDebug()<<"服务器点击发送";
            if(ui->radioButton->isChecked())
            {
                QString str=tcpServerManagementList.at(currIndex)->server->textEdit->toPlainText();
                tcpServerManagementList.at(currIndex)->server->
                        sendMsg(str,ui->comboBox_TCPclients->currentText());
            }
            else {
                QString str=tcpServerManagementList.at(currIndex)->server->hexEdit->toPlainText();
                tcpServerManagementList.at(currIndex)->server->
                        sendHexMsg(str,ui->comboBox_TCPclients->currentText());
            }
        }
    }
    else if (parentName=="TCP客户端") {
        if(tcpClientList.at(currIndex)->isConnection)
        {
            if(ui->radioButton->isChecked())
            {
                QString str=tcpClientList.at(currIndex)->textEdit->toPlainText();
                tcpClientList.at(currIndex)->sendMsg(str);
            }
            else {
                QString str=tcpClientList.at(currIndex)->hexEdit->toPlainText();
                tcpClientList.at(currIndex)->sendHexMsg(str);
            }
        }
    }
    else if (parentName=="UDP服务端") {
        if(!udpServerList.at(currIndex)->getOtherIP().isEmpty())
        {
            if(ui->radioButton->isChecked())
            {
                QString str=udpServerList.at(currIndex)->textEdit->toPlainText();
                udpServerList.at(currIndex)->sendMsg(str);
                ui->label_sendNum->setText(QString::number(udpServerList.at(currIndex)->getSendBit())+" 字节");
            }
            else {
                QString str=udpServerList.at(currIndex)->hexEdit->toPlainText();
                udpServerList.at(currIndex)->sendHexMsg(str);
                ui->label_sendNum->setText(QString::number(udpServerList.at(currIndex)->getSendBit())+" 字节");
            }
        }
    }
    else if (parentName=="UDP客户端") {
        if(!udpClientList.at(currIndex)->getOtherIP().isEmpty())
        {
            if(ui->radioButton->isChecked())
            {
                qDebug()<<"UDP客户端发送不为空";
                QString str=udpClientList.at(currIndex)->textEdit->toPlainText();
                udpClientList.at(currIndex)->sendMsg(str);
                ui->label_sendNum->setText(QString::number(udpClientList.at(currIndex)->getSendBit())+" 字节");
            }
            else {
                qDebug()<<"UDP客户端发送不为空";
                QString str=udpClientList.at(currIndex)->hexEdit->toPlainText();
                udpClientList.at(currIndex)->sendHexMsg(str);
                ui->label_sendNum->setText(QString::number(udpClientList.at(currIndex)->getSendBit())+" 字节");
            }
        }
    }
    else if (parentName=="串口(自动检测，无需创建)") {
        if(serialPortList.at(currIndex)->getState())
        {
            if(ui->radioButton->isChecked())
            {
                qDebug()<<"串口已经打开，可以发送";
                QString str=serialPortList.at(currIndex)->textEdit->toPlainText();
                serialPortList.at(currIndex)->sendMsg(str);
                ui->label_sendNum->setText(QString::number(serialPortList.at(currIndex)->getSendBit())+" 字节");
            }
            else {
                qDebug()<<"串口已经打开，可以发送";
                QString str=serialPortList.at(currIndex)->hexEdit->toPlainText();
                serialPortList.at(currIndex)->sendHexMsg(str);
                ui->label_sendNum->setText(QString::number(serialPortList.at(currIndex)->getSendBit())+" 字节");
            }
        }
    }
}

//定时发送,在点击后更改按钮信息，在每个类中增加定时发送状态，正在发送或没有发送
void MainWindow::timerSendDate()
{
    if(ui->lineEdit_time->text().toInt()<0&&ui->lineEdit_time->text().toInt()>9999999)
    {
        QMessageBox::warning(nullptr,"Error","时间间隔超出范围。\n0<time(ms)<999999");
        return;
    }
    int currIndex=ui->treeView->currentIndex().row();
    //通过判断当前所选中item的parent来更改相应的标签
    if(ui->treeView->currentIndex().parent().data().toString()=="TCP服务器")
    {
        if(tcpServerManagementList.at(currIndex)->isListening())
        {
            if(!tcpServerManagementList.at(currIndex)->server->isTimerSending())
            {
                ui->toolButton_timerStartstop->setText("停止发送");
                if(!tcpServerManagementList.at(currIndex)->isHaveServer)
                {
                    return;
                }
                if(ui->radioButton->isChecked())
                {
                    QString msg=tcpServerManagementList.at(currIndex)->server->textEdit->toPlainText();
                    float s=ui->lineEdit_time->text().toFloat();
                    int count=ui->spinBox->value();
                    QString addr=ui->comboBox_TCPclients->currentText();
                    tcpServerManagementList.at(currIndex)->server->timerSend(msg,s,count,10,addr);
                }
                else {
                    QString msg=tcpServerManagementList.at(currIndex)->server->hexEdit->toPlainText();
                    float s=ui->lineEdit_time->text().toFloat();
                    int count=ui->spinBox->value();
                    QString addr=ui->comboBox_TCPclients->currentText();
                    tcpServerManagementList.at(currIndex)->server->timerSend(msg,s,count,16,addr);
                }
            }
            else {
                tcpServerManagementList.at(currIndex)->server->stopTimer();
                ui->toolButton_timerStartstop->setText("定时发送");
            }
        }
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="TCP客户端") {
        if(tcpClientList.at(currIndex)->isConnection)
        {
            if(!tcpClientList.at(currIndex)->isTimerSending())
            {
                ui->toolButton_timerStartstop->setText("停止发送");
                if(tcpClientList.at(currIndex)->isConnection)
                {
                    if(ui->radioButton->isChecked())
                    {
                        QString msg=tcpClientList.at(currIndex)->textEdit->toPlainText();
                        float s=ui->lineEdit_time->text().toFloat();
                        int count=ui->spinBox->value();
                        tcpClientList.at(currIndex)->timerSend(msg,s,count,10);
                    }
                    else {
                        QString msg=tcpClientList.at(currIndex)->hexEdit->toPlainText();
                        float s=ui->lineEdit_time->text().toFloat();
                        int count=ui->spinBox->value();
                        tcpClientList.at(currIndex)->timerSend(msg,s,count,16);
                    }
                }
            }
            else {
                tcpClientList.at(currIndex)->stopTimer();
                ui->toolButton_timerStartstop->setText("定时发送");
            }
        }
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP服务端") {
        if(!udpClientList.at(currIndex)->getOtherIP().isEmpty())
        {
            if(!udpServerList.at(currIndex)->isTimerSending())
            {
                qDebug()<<"udpjinru";
                ui->toolButton_timerStartstop->setText("停止发送");
                if(ui->radioButton->isChecked())
                {
                    QString msg=udpServerList.at(currIndex)->textEdit->toPlainText();
                    float s=ui->lineEdit_time->text().toFloat();
                    int count=ui->spinBox->value();
                    udpServerList.at(currIndex)->timerSend(msg,s,count,10);
                }
                else {
                    QString msg=udpServerList.at(currIndex)->hexEdit->toPlainText();
                    float s=ui->lineEdit_time->text().toFloat();
                    int count=ui->spinBox->value();
                    udpServerList.at(currIndex)->timerSend(msg,s,count,16);
                }
            }
            else {
                udpServerList.at(currIndex)->stopTimer();
                ui->toolButton_timerStartstop->setText("定时发送");
            }
        }
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP客户端") {
        if(!udpClientList.at(currIndex)->getOtherIP().isEmpty())
        {
            if(!udpClientList.at(currIndex)->isTimerSending())
            {
                ui->toolButton_timerStartstop->setText("停止发送");
                if(ui->radioButton->isChecked())
                {
                    QString msg=udpClientList.at(currIndex)->textEdit->toPlainText();
                    float s=ui->lineEdit_time->text().toFloat();
                    int count=ui->spinBox->value();
                    udpClientList.at(currIndex)->timerSend(msg,s,count,10);
                }
                else {
                    QString msg=udpClientList.at(currIndex)->hexEdit->toPlainText();
                    float s=ui->lineEdit_time->text().toFloat();
                    int count=ui->spinBox->value();
                    udpClientList.at(currIndex)->timerSend(msg,s,count,16);
                }
            }
            else {
                udpClientList.at(currIndex)->stopTimer();
                ui->toolButton_timerStartstop->setText("定时发送");
            }
        }
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="串口(自动检测，无需创建)") {
        if(serialPortList.at(currIndex)->getState())
        {
            if(!serialPortList.at(currIndex)->isTimerSending())
            {
                ui->toolButton_timerStartstop->setText("停止发送");
                if(serialPortList.at(currIndex)->getState())
                {
                    if(ui->radioButton->isChecked())
                    {
                        QString msg=serialPortList.at(currIndex)->textEdit->toPlainText();
                        float s=ui->lineEdit_time->text().toFloat();
                        int count=ui->spinBox->value();
                        serialPortList.at(currIndex)->timerSend(msg,s,count,10);
                    }
                    else {
                        QString msg=serialPortList.at(currIndex)->hexEdit->toPlainText();
                        float s=ui->lineEdit_time->text().toFloat();
                        int count=ui->spinBox->value();
                        serialPortList.at(currIndex)->timerSend(msg,s,count,16);
                    }
                }
            }
            else {
                serialPortList.at(currIndex)->stopTimer();
                ui->toolButton_timerStartstop->setText("定时发送");
            }
        }
    }
}

//创建TCP服务器
void MainWindow::createTcpServer(int port)
{
    QStandardItem *newItem;
    qDebug()<<"port="<<port;
    newItem=new QStandardItem("TCP_server_"+QString::number(tcp_server_num++)+"("+QString::number(port)+")");
    m_Tree->item(ui->treeView->currentIndex().row())->appendRow(newItem);//添加子节点
    ui->treeView->expand(ui->treeView->currentIndex());//打开当前节点
    tcpServerManagement *newtcpServerManagement=new tcpServerManagement(tcpServerManagementList.length());
    newtcpServerManagement->server->setPort(port);
    connect(newtcpServerManagement->server,&tcpServer::hasClient,this,&MainWindow::updateDisInfo);
    connect(newtcpServerManagement->server,&tcpServer::sendDateSIGNAL,this,&MainWindow::upDateDeviceInfo);
    connect(newtcpServerManagement->server,&tcpServer::sendDisSIGNAL,this,&MainWindow::updateDisInfo);

    tcpServerManagementList.append(newtcpServerManagement);
    ui->label_set_writeip->setText(newtcpServerManagement->getIP());
    ui->label_set_writeport->setText(QString::number(port));
    DeviceNum++;
    QString str=QString::number(DeviceNum)+"/"+QString::number(MaxDeviceNum);
    ui->label_DeviceNum->setText(str);
}

//创建TCP客户端
void MainWindow::createTcpClient(QString Hostip, int Hostport)
{
    QStandardItem *newItem;
    newItem=new QStandardItem("TCP_client_"+QString::number(tcp_client_num++));
    m_Tree->item(ui->treeView->currentIndex().row())->appendRow(newItem);
    ui->treeView->expand(ui->treeView->currentIndex());

    tcpClient *newTcpClient=new tcpClient(Hostip,Hostport,tcpClientList.length());
    connect(newTcpClient,&tcpClient::disconnectSIGNAL,this,&MainWindow::updateDisInfo);
    connect(newTcpClient,&tcpClient::sendMsgSIGNAL,this,&MainWindow::upDateDeviceInfo);
    tcpClientList.append(newTcpClient);
    ui->label_set_writeip->setText(newTcpClient->getMyIP());
    DeviceNum++;
    QString str=QString::number(DeviceNum)+"/"+QString::number(MaxDeviceNum);
    ui->label_DeviceNum->setText(str);
}

//创建UDP服务器
void MainWindow::createUdpServer(int port)
{
    qDebug()<<port;
    QStandardItem *newItem;
    newItem=new QStandardItem("UDP_server_"+QString::number(udp_server_num++));
    m_Tree->item(ui->treeView->currentIndex().row())->appendRow(newItem);
    ui->treeView->expand(ui->treeView->currentIndex());

    udpServer *newUdpServer=new udpServer(port,udpServerList.length());
    udpServerList.append(newUdpServer);
    connect(newUdpServer,&udpServer::ricvBitSIGNAL,this,&MainWindow::upDateDeviceInfo);
    DeviceNum++;
    QString str=QString::number(DeviceNum)+"/"+QString::number(MaxDeviceNum);
    ui->label_DeviceNum->setText(str);
}

//创建UDP客户端
void MainWindow::createUdpClient(QString serverIp, int serverPort)
{
    QStandardItem *newItem;
    newItem=new QStandardItem("UDP_client_"+QString::number(udp_client_num++));
    m_Tree->item(ui->treeView->currentIndex().row())->appendRow(newItem);
    ui->treeView->expand(ui->treeView->currentIndex());

    udpClient *newUdpClient=new udpClient(serverIp,serverPort,udpClientList.length());
    udpClientList.append(newUdpClient);
    connect(newUdpClient,&udpClient::ricvBitSIGNAL,this,&MainWindow::upDateDeviceInfo);
    DeviceNum++;
    QString str=QString::number(DeviceNum)+"/"+QString::number(MaxDeviceNum);
    ui->label_DeviceNum->setText(str);
}

//关于帮助
void MainWindow::aboutHelp()
{
    qDebug()<<"aboutHelp";
    HelpDialog *dialog=new HelpDialog();
    dialog->setModal(true);
    dialog->show();
}

//计数清零
void MainWindow::Retimer()
{
    if(ui->treeView->currentIndex().parent().data().toString()=="TCP服务器")
    {
        if(tcpServerManagementList.at(ui->treeView->currentIndex().row())->isHaveServer)
        tcpServerManagementList.at(ui->treeView->currentIndex().row())->setZeroBit();
        QString ricvbit=QString::number(tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->getRicvBit());
        QString sendbit=QString::number(tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->getSendBit());
        ui->label_ricvNum->setText(ricvbit+" 字节");
        ui->label_sendNum->setText(sendbit+" 字节");
    }
    else if(ui->treeView->currentIndex().parent().data().toString()=="TCP客户端")
    {
        tcpClientList.at(ui->treeView->currentIndex().row())->setZero();
        QString ricvbit=QString::number(tcpClientList.at(ui->treeView->currentIndex().row())->getRicvBit());
        QString sendbit=QString::number(tcpClientList.at(ui->treeView->currentIndex().row())->getSendBit());
        ui->label_ricvNum->setText(ricvbit+" 字节");
        ui->label_sendNum->setText(sendbit+" 字节");
    }
    else if(ui->treeView->currentIndex().parent().data().toString()=="UDP服务器")
    {
        udpServerList.at(ui->treeView->currentIndex().row())->setZero();
        QString ricvbit=QString::number(udpServerList.at(ui->treeView->currentIndex().row())->getRicvBit());
        QString sendbit=QString::number(udpServerList.at(ui->treeView->currentIndex().row())->getSendBit());
        ui->label_ricvNum->setText(ricvbit+" 字节");
        ui->label_sendNum->setText(sendbit+" 字节");
    }
    else if(ui->treeView->currentIndex().parent().data().toString()=="UDP客户端")
    {
        udpClientList.at(ui->treeView->currentIndex().row())->setZero();
        QString ricvbit=QString::number(udpClientList.at(ui->treeView->currentIndex().row())->getRicvBit());
        QString sendbit=QString::number(udpClientList.at(ui->treeView->currentIndex().row())->getSendBit());
        ui->label_ricvNum->setText(ricvbit+" 字节");
        ui->label_sendNum->setText(sendbit+" 字节");
    }
    else if(ui->treeView->currentIndex().parent().data().toString()=="串口(自动检测，无需创建)")
    {
        serialPortList.at(ui->treeView->currentIndex().row())->setZero();
        QString ricvbit=QString::number(serialPortList.at(ui->treeView->currentIndex().row())->getRicvBit());
        QString sendbit=QString::number(serialPortList.at(ui->treeView->currentIndex().row())->getSendBit());
        ui->label_ricvNum->setText(ricvbit+" 字节");
        ui->label_sendNum->setText(sendbit+" 字节");
    }
    qDebug()<<"ReTimer";
}

//更新设备信息(字节数和定时按钮)
void MainWindow::upDateDeviceInfo(int send, int ricv, int falgcount, int type)
{
    qDebug()<<"SLOT";
    switch (type) {
    case 1:
        if(ui->treeView->currentIndex().parent().data().toString()=="TCP服务器")
        {
            qDebug()<<"tcp服务器更新字节和定时按钮";
            if(tcpServerManagementList.at(ui->treeView->currentIndex().row())->getfalgcount()==falgcount)
            {
                ui->label_sendNum->setText(QString::number(send)+" 字节");
                ui->label_ricvNum->setText(QString::number(ricv)+" 字节");
                qDebug()<<"upDateBit"<<send<<ricv;
                if(tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->isTimerSending()==true)
                    ui->toolButton_timerStartstop->setText("停止发送");
                else
                    ui->toolButton_timerStartstop->setText("定时发送");
            }
        }
        break;
    case 2:
        if(ui->treeView->currentIndex().parent().data().toString()=="TCP客户端")
        {
            if(tcpClientList.at(ui->treeView->currentIndex().row())->getflagcount()==falgcount)
            {
                ui->label_sendNum->setText(QString::number(send)+" 字节");
                ui->label_ricvNum->setText(QString::number(ricv)+" 字节");
                qDebug()<<"upDateBit"<<send<<ricv;
                if(tcpClientList.at(ui->treeView->currentIndex().row())->isTimerSending()==true)
                    ui->toolButton_timerStartstop->setText("停止发送");
                else
                    ui->toolButton_timerStartstop->setText("定时发送");
            }
        }
        break;
    case 3:
        if(ui->treeView->currentIndex().parent().data().toString()=="UDP服务端")
        {   
            if(udpServerList.at(ui->treeView->currentIndex().row())->getflagcount()==falgcount)
            {
                ui->label_sendNum->setText(QString::number(send)+" 字节");
                ui->label_ricvNum->setText(QString::number(ricv)+" 字节");
                if(udpServerList.at(ui->treeView->currentIndex().row())->isTimerSending()==true)
                    ui->toolButton_timerStartstop->setText("停止发送");
                else
                    ui->toolButton_timerStartstop->setText("定时发送");
            }
        }
        break;
    case 4:
        if(ui->treeView->currentIndex().parent().data().toString()=="UDP客户端")
        {
            if(udpClientList.at(ui->treeView->currentIndex().row())->getflagcount()==falgcount)
            {
                ui->label_sendNum->setText(QString::number(send)+" 字节");
                ui->label_ricvNum->setText(QString::number(ricv)+" 字节");
                qDebug()<<"upDateBit"<<send<<ricv;
                if(udpClientList.at(ui->treeView->currentIndex().row())->isTimerSending()==true)
                    ui->toolButton_timerStartstop->setText("停止发送");
                else
                    ui->toolButton_timerStartstop->setText("定时发送");
            }
        }
        break;
    case 5:
        qDebug()<<"SLOT";
        if(ui->treeView->currentIndex().parent().data().toString()=="串口(自动检测，无需创建)")
        {
            if(serialPortList.at(ui->treeView->currentIndex().row())->getflagcount()==falgcount)
            {
                ui->label_sendNum->setText(QString::number(send)+" 字节");
                ui->label_ricvNum->setText(QString::number(ricv)+" 字节");
                qDebug()<<"upDateBit"<<send<<ricv;
                if(serialPortList.at(ui->treeView->currentIndex().row())->isTimerSending()==true)
                    ui->toolButton_timerStartstop->setText("停止发送");
                else
                    ui->toolButton_timerStartstop->setText("定时发送");
            }
        }
        break;
    }
}

//槽函数，收到一个客户端断开连接的信号，
//判断，如果是当前所选中的客户端是断开连接的客户端则改变设备信息。
//判断，如果是当前是TCP服务器则重新加载tcp服务器中的客户端combobox
void MainWindow::updateDisInfo()
{
    if(ui->treeView->currentIndex().parent().data().toString()=="TCP服务器")
    {
        ui->comboBox_TCPclients->clear();
        ui->comboBox_TCPclients->addItem("全部客户端");
        if(!tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->getClientList().isEmpty())
        {

            foreach(QString client,tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->getClientList()){
                ui->comboBox_TCPclients->addItem(client);
            }
        }
    }
    else if(ui->treeView->currentIndex().parent().data().toString()=="TCP客户端")
    {
        foreach (tcpClient *client, tcpClientList) {
            if(client->getflagcount()==ui->treeView->currentIndex().row()&&client->isConnection==false)
            {
                ui->label_Device_State->setText("未就绪");
                ui->pushButton_open_close->setText("连接");
            }
        }
    }
}

void MainWindow::HexEditSLOT()
{
    if(ui->treeView->currentIndex().parent().data().toString()=="TCP服务器")
    {
        tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textEdit->hide();
        tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textEdit->setParent(nullptr);
        tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->setParent(ui->groupBox_send);
        tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->show();
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="TCP客户端") {
        tcpClientList.at(ui->treeView->currentIndex().row())->textEdit->hide();
        tcpClientList.at(ui->treeView->currentIndex().row())->textEdit->setParent(nullptr);
        tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(ui->groupBox_send);
        tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->show();
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP服务端") {
        udpServerList.at(ui->treeView->currentIndex().row())->textEdit->hide();
        udpServerList.at(ui->treeView->currentIndex().row())->textEdit->setParent(nullptr);
        udpServerList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(ui->groupBox_send);
        udpServerList.at(ui->treeView->currentIndex().row())->hexEdit->show();
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP客户端") {
        udpClientList.at(ui->treeView->currentIndex().row())->textEdit->hide();
        udpClientList.at(ui->treeView->currentIndex().row())->textEdit->setParent(nullptr);
        udpClientList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(ui->groupBox_send);
        udpClientList.at(ui->treeView->currentIndex().row())->hexEdit->show();
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="串口(自动检测，无需创建)") {
        serialPortList.at(ui->treeView->currentIndex().row())->textEdit->hide();
        serialPortList.at(ui->treeView->currentIndex().row())->textEdit->setParent(nullptr);
        serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(ui->groupBox_send);
        serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->show();
    }
}

//清空接收框数据
void MainWindow::clearDataSLOT()
{
    if(ui->treeView->currentIndex().parent().data().toString()=="TCP服务器")
    {
        if(tcpServerManagementList.at(ui->treeView->currentIndex().row())->isListening())
        {
            tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->clearData();
        }
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="TCP客户端")
    {
        if(tcpClientList.at(ui->treeView->currentIndex().row())->isConnection)
        {
            tcpClientList.at(ui->treeView->currentIndex().row())->clearData();
        }
    }
    else if(ui->treeView->currentIndex().parent().data().toString()=="UDP服务端")
    {
        if(!udpServerList.at(ui->treeView->currentIndex().row())->getOtherIP().isEmpty())
        {
            udpServerList.at(ui->treeView->currentIndex().row())->clearData();
        }
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP客户端")
    {
        if(!udpClientList.at(ui->treeView->currentIndex().row())->getOtherIP().isEmpty())
        {
            udpClientList.at(ui->treeView->currentIndex().row())->clearData();
        }
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="串口(自动检测，无需创建)")
    {
        if(serialPortList.at(ui->treeView->currentIndex().row())->getState())
        {
            serialPortList.at(ui->treeView->currentIndex().row())->clearData();
        }
    }
}

//保存数据到文件
void MainWindow::saveDataSLOT()
{
    if(ui->treeView->currentIndex().parent().data().toString()=="TCP服务器")
    {
        tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->saveData();
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="TCP客户端")
    {
        tcpClientList.at(ui->treeView->currentIndex().row())->saveData();
    }
    else if(ui->treeView->currentIndex().parent().data().toString()=="UDP服务端")
    {
        udpServerList.at(ui->treeView->currentIndex().row())->saveData();
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP客户端")
    {
        udpClientList.at(ui->treeView->currentIndex().row())->saveData();
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="串口(自动检测，无需创建)")
    {
        serialPortList.at(ui->treeView->currentIndex().row())->saveData();
    }
}

void MainWindow::clearSendSLOT()
{
    int type=0;
    if(ui->radioButton->isChecked())
        type=10;
    if(ui->radioButton_2->isChecked())
        type=16;
    if(ui->treeView->currentIndex().parent().data().toString()=="TCP服务器")
    {
        if(tcpServerManagementList.at(ui->treeView->currentIndex().row())->isListening())
            tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->clearSend(type);
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="TCP客户端")
    {
        if(tcpClientList.at(ui->treeView->currentIndex().row())->isConnection)
            tcpClientList.at(ui->treeView->currentIndex().row())->clearSend(type);
    }
    else if(ui->treeView->currentIndex().parent().data().toString()=="UDP服务端")
    {
        if(!udpServerList.at(ui->treeView->currentIndex().row())->getOtherIP().isEmpty())
            udpServerList.at(ui->treeView->currentIndex().row())->clearSend(type);
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP客户端")
    {
        if(!udpClientList.at(ui->treeView->currentIndex().row())->getOtherIP().isEmpty())
            udpClientList.at(ui->treeView->currentIndex().row())->clearSend(type);
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="串口(自动检测，无需创建)")
    {
        if(serialPortList.at(ui->treeView->currentIndex().row())->getState())
            serialPortList.at(ui->treeView->currentIndex().row())->clearSend(type);
    }
}

void MainWindow::loadFileSLOT()
{
    int type=0;
    if(ui->radioButton->isChecked())
        type=10;
    if(ui->radioButton_2->isChecked())
        type=16;
    if(ui->treeView->currentIndex().parent().data().toString()=="TCP服务器")
    {
        if(tcpServerManagementList.at(ui->treeView->currentIndex().row())->isListening())
        {
            if(ui->pushButton_loadFile->text()=="加载文件")
            {
                QString filePath = QFileDialog::getOpenFileName();
                ui->pushButton_loadFile->setText("停止加载");
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->loadFile(filePath,type);
            }
            else {
                ui->pushButton_loadFile->setText("加载文件");
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textEdit->setEnabled(true);
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->setEnabled(true);
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->isFile=false;
                tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->isFile=false;
            }
        }
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="TCP客户端")
    {
        if(tcpClientList.at(ui->treeView->currentIndex().row())->isConnection)
        {
            if(ui->pushButton_loadFile->text()=="加载文件")
            {
                QString filePath = QFileDialog::getOpenFileName();
                ui->pushButton_loadFile->setText("停止加载");
                tcpClientList.at(ui->treeView->currentIndex().row())->loadFile(filePath,type);
            }
            else
            {
                ui->pushButton_loadFile->setText("加载文件");
                tcpClientList.at(ui->treeView->currentIndex().row())->textEdit->setEnabled(true);
                tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->setEnabled(true);
                tcpClientList.at(ui->treeView->currentIndex().row())->isFile=false;
                tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->isFile=false;
            }
        }
    }
    else if(ui->treeView->currentIndex().parent().data().toString()=="UDP服务端")
    {
        if(!udpServerList.at(ui->treeView->currentIndex().row())->getOtherIP().isEmpty())
        {
            if(ui->pushButton_loadFile->text()=="加载文件")
            {
                QString filePath = QFileDialog::getOpenFileName();
                ui->pushButton_loadFile->setText("停止加载");
                udpServerList.at(ui->treeView->currentIndex().row())->loadFile(filePath,type);
            }
            else
            {
                ui->pushButton_loadFile->setText("加载文件");
                udpServerList.at(ui->treeView->currentIndex().row())->textEdit->setEnabled(true);
                udpServerList.at(ui->treeView->currentIndex().row())->hexEdit->setEnabled(true);
                udpServerList.at(ui->treeView->currentIndex().row())->isFile=false;
                udpServerList.at(ui->treeView->currentIndex().row())->hexEdit->isFile=false;
            }
        }
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP客户端")
    {
        if(!udpClientList.at(ui->treeView->currentIndex().row())->getOtherIP().isEmpty())
        {
            if(ui->pushButton_loadFile->text()=="加载文件")
            {
                QString filePath = QFileDialog::getOpenFileName();
                ui->pushButton_loadFile->setText("停止加载");
                udpClientList.at(ui->treeView->currentIndex().row())->loadFile(filePath,type);
            }
            else
            {
                ui->pushButton_loadFile->setText("加载文件");
                udpClientList.at(ui->treeView->currentIndex().row())->textEdit->setEnabled(true);
                udpClientList.at(ui->treeView->currentIndex().row())->hexEdit->setEnabled(true);
                udpClientList.at(ui->treeView->currentIndex().row())->isFile=false;
                udpClientList.at(ui->treeView->currentIndex().row())->hexEdit->isFile=false;
            }
        }
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="串口(自动检测，无需创建)")
    {
        if(serialPortList.at(ui->treeView->currentIndex().row())->getState())
        {
            if(ui->pushButton_loadFile->text()=="加载文件")
            {
                QString filePath = QFileDialog::getOpenFileName();
                ui->pushButton_loadFile->setText("停止加载");
                serialPortList.at(ui->treeView->currentIndex().row())->loadFile(filePath,type);
            }
            else
            {
                ui->pushButton_loadFile->setText("加载文件");
                serialPortList.at(ui->treeView->currentIndex().row())->textEdit->setEnabled(true);
                serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->setEnabled(true);
                serialPortList.at(ui->treeView->currentIndex().row())->isFile=false;
                serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->isFile=false;
            }
        }
    }
}

void MainWindow::clickSetting()
{
    SettingDialog *a=new SettingDialog(this);
    connect(a,&SettingDialog::sendDeviceNumSIGNAL,this,&MainWindow::setMaxDeviceNum);
    a->show();
}

void MainWindow::AsciiEditSLOT()
{
    if(ui->treeView->currentIndex().parent().data().toString()=="TCP服务器")
    {
        tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->hide();
        tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->hexEdit->setParent(nullptr);
        tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textEdit->setParent(ui->groupBox_send);
        tcpServerManagementList.at(ui->treeView->currentIndex().row())->server->textEdit->show();
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="TCP客户端") {
        tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->hide();
        tcpClientList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(nullptr);
        tcpClientList.at(ui->treeView->currentIndex().row())->textEdit->setParent(ui->groupBox_send);
        tcpClientList.at(ui->treeView->currentIndex().row())->textEdit->show();
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP服务端") {
        udpServerList.at(ui->treeView->currentIndex().row())->hexEdit->hide();
        udpServerList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(nullptr);
        udpServerList.at(ui->treeView->currentIndex().row())->textEdit->setParent(ui->groupBox_send);
        udpServerList.at(ui->treeView->currentIndex().row())->textEdit->show();
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="UDP客户端") {
        udpClientList.at(ui->treeView->currentIndex().row())->hexEdit->hide();
        udpClientList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(nullptr);
        udpClientList.at(ui->treeView->currentIndex().row())->textEdit->setParent(ui->groupBox_send);
        udpClientList.at(ui->treeView->currentIndex().row())->textEdit->show();
    }
    else if (ui->treeView->currentIndex().parent().data().toString()=="串口(自动检测，无需创建)") {
        serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->hide();
        serialPortList.at(ui->treeView->currentIndex().row())->hexEdit->setParent(nullptr);
        serialPortList.at(ui->treeView->currentIndex().row())->textEdit->setParent(ui->groupBox_send);
        serialPortList.at(ui->treeView->currentIndex().row())->textEdit->show();
    }
}

