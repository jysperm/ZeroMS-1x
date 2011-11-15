#ifndef CLIENTCONN_H
#define CLIENTCONN_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QString>
#include <QDateTime>
#include "const.h"

class ClientConn:public QObject
{
    Q_OBJECT
    //该类是对QTcpSocket的一个简单封装，目的是为了使QTcpSocket加入一些于客户连接有关的信息，
    //例如上次请求时间等等
    //只是简单的添加了几个共有成员变量,简单到连实现文件都没有
public:
    //用户名，冗余数据，SocketList将自动保证它和在SocketList中的键名保持同步
    //注意，只有uname是SocketList自动维护的，其他请手动设置
    QString uname;
    //是否已经登录成功
    int isLoged;
    //是否已经被关闭
    int isClose;
    //上次请求时间，客户端最后一次给服务器发消息的时间，而不是服务器最后给客户端发消息的时间
    unsigned int lasttime;
    //客户端版本顺序号
    int clientversion;
    //客户端名称描述性字符串
    QString clientname;
    //数据缓冲
    QByteArray *data;
    //在期待多长的数据，冗余数据,为了加速判断是否有需要读取的信息,为0表示还没有接受到数据头
    int waitForLen;
    //连接对象
    QTcpSocket *conn;

    //构造函数，因为内容不多，就不单独用.cpp文件了
    ClientConn()
    {
        //一开始期待一个命令头部
        waitForLen=P1_HEAD_LEN;
        lasttime=QDateTime::currentDateTime().toTime_t();
        isClose=0;
        isLoged=0;
        clientversion=0;
    }
};

#endif // CLIENTCONN_H
