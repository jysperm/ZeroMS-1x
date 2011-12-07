#ifndef PUBLIC_OPACKET_H
#define PUBLIC_OPACKET_H

#include <QByteArray>
#include <QObject>
#include "const.h"
#include "p1.h"
using namespace P1;

class OPacket:public QObject
{
    //对数据包的封装，表示一个数据包
    //因为只是简单的封装，所以所有成员共有

    //内容不多,所以不单独使用实现文件了
public:
    OPacket(QByteArray &data,unsigned int type);
    OPacket(unsigned int type);
    //返回一个可以用于发送的完整的数据包
    QByteArray exec();
    //把数据追加到数据包的尾部，事实上只是一个简写形式
    inline OPacket &append(QByteArray &data);

    unsigned int packetType;
    QByteArray packetData;
};

inline OPacket &OPacket::append(QByteArray &data)
{
    packetData.append(data);
    return *this;
}

#endif // PUBLIC_OPACKET_H
