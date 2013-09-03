#ifndef CLIENTCONN_H
#define CLIENTCONN_H

#include "const.h"
#include "../public/OPacket.h"

class OClient:public QObject
{
    //该类是对客户端连接的一个简单抽象，一个Client对象代表一个客户端
    //事实上该类只是添加了一些数据成员，用来为连接对象提供一些额外的信息

    //只是简单的添加了几个共有成员变量,所以不单独使用实现文件了
public:
    OClient();
    virtual ~OClient();
    inline unsigned int ping();//将最后登陆时间改为当前时间
    inline void send(OPacket &packet);

    int isLoged;//是否已经登录成功
    unsigned int lasttime;//上次请求时间，客户端最后一次给服务器发消息的时间
    int clientver;//客户端版本顺序号
    QString clientname;//客户端名称描述性字符串
    QByteArray *databuf;//数据缓冲
    QTcpSocket *conn;//连接对象
};

inline unsigned int OClient::ping()
{
    return lasttime=QDateTime::currentDateTime().toTime_t();
}

inline void OClient::send(OPacket &packet)
{
    conn->write(packet.exec());
}

#endif // CLIENTCONN_H
