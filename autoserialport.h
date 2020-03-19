#ifndef AUTOSERIALPORT_H
#define AUTOSERIALPORT_H

#include <QObject>
#include <QThread>
#include <QTimer>
class autoSerialPort : public QThread
{
    Q_OBJECT
public:
    explicit autoSerialPort(QObject *parent = nullptr);
    void run();


private:
    QTimer *timer;

signals:
    void sendSerialName(QStringList);
public slots:
    void outTime();
};

#endif // AUTOSERIALPORT_H
