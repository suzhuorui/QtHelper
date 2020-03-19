#include "autoserialport.h"
#include <QDebug>

autoSerialPort::autoSerialPort(QObject *parent) : QThread(parent)
{

}

void autoSerialPort::run()
{

    exec();
}

void autoSerialPort::outTime()
{

    emit sendSerialName(SerialPortNameList);
}
