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
    OMessage(int type);
    //注意这个是从二进制数据构造，而不是把参数作为消息的数据不恩
    OMessage(QByteArray data);

    static OMessage fromDataBuff(QByteArray *databuf,bool autoRemove=true);

    inline bool isEmpty();
    inline OMessage &append(QByteArray data);
    inline OMessage &append(QString data);
    inline OMessage &aSpc();
    inline static int QBtoint(QByteArray b);
    QByteArray exec();

    QString split(int n);//返回以空格分割的字符串的第n项(从0开始)
    QString splitTail(int n);//返回以空格分割的字符串的第n项以及之后的所有字符(从0开始)

    int protocol;
    int type;
    unsigned int time;
    QByteArray data;
};

inline bool OMessage::isEmpty()
{
    if((!type) && data.isEmpty())
        return true;
    else
        return false;
}

inline OMessage &OMessage::append(QByteArray data)
{
    this->data.append(data);
    return *this;
}

inline OMessage &OMessage::append(QString data)
{
    this->data.append(data);
    return *this;
}

inline OMessage &OMessage::aSpc()
{
    data.append(" ");
    return *this;
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
