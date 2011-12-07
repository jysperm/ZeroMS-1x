#include <QDataStream>
#include <QDateTime>
#include "opacket.h"
#include "const.h"

OPacket::OPacket(QByteArray &data,unsigned int type):packetType(type),packetData(data)
{

}

OPacket::OPacket(unsigned int type):packetType(type)
{

}

QByteArray OPacket::exec()
{
    if(!packetType)
        return 0;
    QByteArray tData;
    QDataStream DSdata(&tData,QIODevice::WriteOnly);
    unsigned int time=QDateTime::currentDateTime().toTime_t();
    DSdata<<P_VER<<packetData.size()<<packetType<<time;
    tData.append(packetData);
    return tData;
}

inline OPacket &OPacket::append(QByteArray &data)
{
    packetData.append(data);
    return *this;
}
