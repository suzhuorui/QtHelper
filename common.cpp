#include "common.h"
#include <QDebug>
common::common(QWidget *parent) : QTextEdit(parent)
{
    connect(this,&QTextEdit::cursorPositionChanged,this,&common::changePosSLOT);
}

QString common::getCurrTime()
{
    return QTime::currentTime().toString();
}

QByteArray common::hexStrToByteArray(const QString &str)
{
    QByteArray senddata;
    int hexdata, lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len / 2);
    char lstr, hstr;

    for (int i = 0; i < len;) {
        hstr = str.at(i).toLatin1();

        if (hstr == ' ') {
            i++;
            continue;
        }

        i++;

        if (i >= len) {
            break;
        }

        lstr = str.at(i).toLatin1();
        hexdata = convertHexChar(hstr);
        lowhexdata = convertHexChar(lstr);

        if ((hexdata == 16) || (lowhexdata == 16)) {
            break;
        } else {
            hexdata = hexdata * 16 + lowhexdata;
        }

        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }

    senddata.resize(hexdatalen);
    return senddata;
}

char common::convertHexChar(char ch)
{
    if ((ch >= '0') && (ch <= '9')) {
        return ch - 0x30;
    } else if ((ch >= 'A') && (ch <= 'F')) {
        return ch - 'A' + 10;
    }else {
        return (-1);
    }
}

//只能输入16进制数和回退符
void common::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_0||event->key()==Qt::Key_1||event->key()==Qt::Key_2||event->key()==Qt::Key_3||event->key()==Qt::Key_4||event->key()==Qt::Key_5||event->key()==Qt::Key_6||event->key()==Qt::Key_7||event->key()==Qt::Key_8||event->key()==Qt::Key_9||event->key()==Qt::Key_A||event->key()==Qt::Key_B||event->key()==Qt::Key_C||event->key()==Qt::Key_D||event->key()==Qt::Key_E||event->key()==Qt::Key_F)
    {
        QString str=this->toPlainText();
        str.append(event->text().toUpper());
        this->clear();
        this->append(str);
    }
    else if(event->key()==Qt::Key_Backspace) {
        QString str=this->toPlainText();
        QString str1=str.remove(str.length()-1,1);
        this->clear();
        this->append(str1);
    }
}

//限制中文输入
void common::changePosSLOT()
{
    QString str=this->toPlainText();
    if(str=="")
        return;
    QString ch=str.at(str.length()-1);
    qDebug()<<"str="<<str<<"  ch="<<ch;
    if((ch>='A')&&(ch<='F'))
    {
        //ch正常AF";
    }
    else if ((ch>='0')&&(ch<='9')) {
        //正常09";
    }
    else {
        //ch错误";
        if(!isFile)
        {
            str.remove(str.length()-1,1);
            this->clear();
            this->append(str);
        }
    }
}
