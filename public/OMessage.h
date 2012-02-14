#ifndef PUBLIC_OMESSAGE_H
#define PUBLIC_OMESSAGE_H

#include <QObject>
#include <QDataStream>
#include "global.h"

class OMessage
{
public:
    OMessage();
    OMessage(int type,QByteArray data);
    //注意这个是从二进制数据构造，而不是把参数作为消息的数据不恩
    OMessage(QByteArray data);


    static OMessage fromDataBuff(QByteArray *databuf,bool autoRemove=true);

    inline bool isEmpty();
    inline static int QBtoint(QByteArray b);
    QByteArray exec();

    QString split(int n);//返回以空格分割的字符串的第n项(从0开始)
    QString splitTail(int n);//返回以空格分割的字符串的第n项以及之后的所有字符(从0开始)

    QByteArray data;
    unsigned int time;
    int protocol;
    int type;
};

inline bool OMessage::isEmpty()
{
    if((!type) && data.isEmpty())
        return true;
    else
        return false;
}

inline int OMessage::QBtoint(QByteArray b)
{
    //将参数的前4个字节转换成int
    QDataStream d(b);
    int i;
    d>>i;
    return i;
}

#endif // PUBLIC_OMESSAGE_H
