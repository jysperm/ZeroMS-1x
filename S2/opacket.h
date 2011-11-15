#ifndef OPACKET_H
#define OPACKET_H

#include <QByteArray>
#include <QObject>
#include "const.h"

class OPacket:public QObject
{
    //对数据包的封装，表示一个数据包
    //因为只是简单的封装，所以所有成员共有
    Q_OBJECT
public:
    OPacket(QByteArray &data,unsigned int type=0);
    OPacket(unsigned int type=0);
    //返回一个可以用于发送的完整的数据包
    QByteArray exec();
    inline OPacket &append(QByteArray &data)
    {
	//把数据追加到数据包的尾部，事实上只是一个简写形式
	packetData.append(data);
	return *this;
    }

    unsigned int packetType;
    QByteArray packetData;
};

#endif // OPACKET_H
