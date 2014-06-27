#include "OPacket.h"
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

QString OPacket::split(int n)
{
    QString str(data);
    QString temp;
    while(n>-1)
    {
        str.remove(0,temp.length()?temp.length()+1:0);
        temp=str.left(str.indexOf(" "));
        n--;
    }
    return temp;
}

QString OPacket::splitTail(int n)
{
    int N=n;
    int len=0;
    n--;
    while(n>-1)
    {
        len+=split(n).length();
        n--;
    }
    len+=N;

    QString str(data);
    return str.right(str.length()-len);
}
