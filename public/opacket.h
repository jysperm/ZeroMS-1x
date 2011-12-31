#ifndef PUBLIC_OPACKET_H
#define PUBLIC_OPACKET_H

#include <QByteArray>
#include <QObject>
#include "const.h"
#include "p1.h"
using namespace P1;

class OPacket
{
    //对数据包的封装，表示一个数据包
public:
    OPacket(QByteArray d,int t);
    OPacket(int type);
    QByteArray exec();//返回一个可以用于发送的完整的数据包
    inline OPacket &append(QByteArray d);//把数据追加到数据包的尾部
    inline int length();
    unsigned int time;
    int type;
    QByteArray data;
};

inline OPacket &OPacket::append(QByteArray d)
{
    data.append(d);
    return *this;
}

inline int OPacket::length()
{
    return data.size();
}

#endif // PUBLIC_OPACKET_H
