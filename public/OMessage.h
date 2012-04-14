#ifndef PUBLIC_OMESSAGE_H
#define PUBLIC_OMESSAGE_H

#include <QByteArray>
#include <QString>
#include <QDataStream>

class OMessage
{
    //值类
    //该类抽象SB协议中的一个消息，提供了与消息类型无关的解析消息的能力
public:
    OMessage();
    //创建一个类型为type，数据部分为data的对象
    OMessage(int type,QByteArray data);
    OMessage(int type);
    //注意这个是从二进制数据构造，而不是把参数作为消息的数据部分
    OMessage(QByteArray data);

    static OMessage fromDataBuff(QByteArray *databuf,bool autoRemove=true);

    //是否为空,判定依据是type和data同时为空
    inline bool isEmpty() const;

    inline OMessage &append(QString data);//向data末尾添加数据
    inline OMessage &aSpc();//向data末尾添加空格

    QString split(int n);//返回以空格分割的字符串的第n项(从0开始)
    QString splitTail(int n);//返回以空格分割的字符串的第n项以及之后的所有字符(从0开始)

    QByteArray exec();//返回当该消息被发送时应该发送的数据

    int type;
private:
    inline static int QBtoint(QByteArray b);//将参数的前4个字节转换成int

    //见 http://0-ms.org/wiki/show/SB2.0#.E6.B6.88.E6.81.AF.E5.A4.B4.E9.83.A8
    int protocol;
    unsigned int time;
    QByteArray data;
};

inline bool OMessage::isEmpty() const
{
    if((!type) && data.isEmpty())
        return true;
    else
        return false;
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
    QDataStream d(b);
    int i;
    d>>i;
    return i;
}

#endif // PUBLIC_OMESSAGE_H
