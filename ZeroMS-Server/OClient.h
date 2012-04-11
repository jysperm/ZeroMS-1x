#ifndef OCLIENT_H
#define OCLIENT_H

#include <QHostAddress>
#include "OClientPeer.h"

class OClient:public QObject
{
    //该类抽象一个客户端对象
    Q_OBJECT
public:
    explicit OClient();
    virtual ~OClient();

    //获得唯一标识,登录后等同用户名，未登录为  #IP:端口
    //等于在core->cl中的键名
    inline QString getSignature();

    void init();//初始化，绑定主连接的信号槽
    void addSubConn(OClientPeer *peer);//增加次要连接，自动绑定信号槽

    QString uname;//用户名
    bool isLoged;//是否已经登录
    bool isShowIp;//是否向其他用户展示IP
    QString status;//在线状态

    OClientPeer *main;//主连接
    QList<OClientPeer*> subConnList;//次要连接数组
    QVector<int> p2pPorts;
    QMap<QString,QVector<OUserlistItem> > userlistCache;
public slots:
    void onError(OAbstractPeer *peer,QString msg,QAbstractSocket::SocketError s);
signals:
    void lostMainConnect(OClient *client);
};

inline bool OClientPeer::isMain()
{
    return this==client->main;
}

inline QString OClient::getSignature()
{
    if(isLoged)
        return uname;
    else
        return QString("#%1:%2").arg(main->conn->peerAddress().toString()).arg(main->conn->peerPort());
}

#endif // OCLIENT_H
