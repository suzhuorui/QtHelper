#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QString>
#include <QTime>
#include <QKeyEvent>
#include <QTextEdit>
#include <QWidget>
class common : public QTextEdit
{
    Q_OBJECT
public:
    explicit common(QWidget *parent = nullptr);
    static QString getCurrTime();
    static QByteArray hexStrToByteArray(const QString &str);
    static char convertHexChar(char ch);
    bool isFile=false;

protected:
    void keyPressEvent(QKeyEvent *event);
signals:

public slots:
    void changePosSLOT();
};

#endif // COMMON_H
