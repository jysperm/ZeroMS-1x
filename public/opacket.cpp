#include <QDataStream>
#include <QDateTime>
#include "opacket.h"
#include "const.h"

OPacket::OPacket(QByteArray d,int t):type(t),data(d)
{

}

OPacket::OPacket(int t):type(t)
{

}

QByteArray OPacket::exec()
{
    if(!type)
        return 0;
    QByteArray tData;
    QDataStream DSdata(&tData,QIODevice::WriteOnly);
    unsigned int time=QDateTime::currentDateTime().toTime_t();
    DSdata<<P_VER<<data.size()<<type<<time;
    tData.append(data);
    return tData;
}
