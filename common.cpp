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
    /*qDebug()<<"hexStrToByteArray::str="<<str;
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
        qDebug()<<"hexStrToByteArray::hexdata="<<str;
        qDebug()<<"hexStrToByteArray::lowhexdata="<<str;
        if ((hexdata == 16) || (lowhexdata == 16)) {
            break;
        } else {
            hexdata = hexdata * 16 + lowhexdata;
        }

        i++;
        qDebug()<<"hexStrToByteArray::data="<<(char)hexdata;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }

    senddata.resize(hexdatalen);
    */
        bool ok;
        QByteArray ret;
        QString str1;
        QStringList sl;
        QString str2;
        qDebug()<<"str.length="<<str.length();
        for(int i=1;i<=str.length();i++)
        {
            qDebug()<<"i="<<i;
            str2.append(str.at(i-1));
            qDebug()<<"str2="<<str2;
            if(i%2==0)//每两个字符放一起
            {
                sl.append(str2);
                str2.clear();
            }
        }

        foreach (QString s, sl) {
            if(!s.isEmpty())
            {
                //byte类型的数字要&0xff再赋值给int类型，其本质原因就是想保持二进制补码的一致性。
                char c = s.toInt(&ok,16)&0xFF;//任何一个数&0xFF都是它本身，这里为了保持补码的一致性（由byte(8位)-》int(32位)）
                char d= 10;
                qDebug()<<"d="<<d;
                if(ok){
                    ret.append(c);
                }else{
                    qDebug()<<"非法的16进制字符："<<s;
                }
            }
        }
    return ret;
}

/*char common::convertHexChar(char ch)
{
    if ((ch >= '0') && (ch <= '9')) {
        return ch - 0x30;
    } else if ((ch >= 'A') && (ch <= 'F')) {
        return ch - 'A' + 10;
    }else {
        return (-1);
    }
}*/

//只能输入16进制数和回退符
void common::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_0||event->key()==Qt::Key_1||event->key()==Qt::Key_2||event->key()==Qt::Key_3||event->key()==Qt::Key_4||event->key()==Qt::Key_5||event->key()==Qt::Key_6||event->key()==Qt::Key_7||event->key()==Qt::Key_8||event->key()==Qt::Key_9||event->key()==Qt::Key_A||event->key()==Qt::Key_B||event->key()==Qt::Key_C||event->key()==Qt::Key_D||event->key()==Qt::Key_E||event->key()==Qt::Key_F)
    {
        QString str=this->toPlainText();//保存已经有的
        str.append(event->text().toUpper());//添加新传来的
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
    QChar ch1=str.at(str.length()-1);
    char ch2=ch1.toLatin1();
    qDebug()<<"str="<<str<<"  ch="<<ch;
    if((ch2>='A')&&(ch2<='F'))
    {
        //ch正常AF";
    }
    else if ((ch2>='0')&&(ch2<='9')) {
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
