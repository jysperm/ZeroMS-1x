#ifndef SOCKETLIST_H
#define SOCKETLIST_H

#include <QTcpSocket>
#include <QMap>
#include <QString>
#include "const.h"
#include "clientconn.h"

class SocketList:public QObject,public QMap<QString,ClientConn*>
{
    Q_OBJECT
    //该类用来维护一个键值对应的表，用来储存连接对象QTcpSocket
    //该类是对QMap的一个简单封装
public:
    SocketList();
    //添加元素
    ClientConn *add(QString,QTcpSocket*);
    //更改键名
    ClientConn *rename(QString,QString);
    //删除元素(自动释放内存)
    int remove(QString);
private:
};

#endif // SOCKETLIST_H
