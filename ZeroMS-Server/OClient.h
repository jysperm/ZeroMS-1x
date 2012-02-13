#ifndef OCLIENT_H
#define OCLIENT_H

#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <QVector>
#include "global.h"
#include "../public/OMessage.h"

class OClient:public QObject
{
    //该类抽象一个客户端对象
    Q_OBJECT
public:
    class Connect
    {
        //该类抽象一个客户端连接(SB2.x是允许多连接的)
    public:
        Connect();
        Connect(OClient *client);
        Connect(QTcpSocket *conn,OClient *client);
        QTcpSocket *conn;//连接对象
        QByteArray databuf;//数据缓冲
        OClient *client;//属于哪个客户端对象
        QString publicKey;//公钥
        inline bool isMain();
        inline void send(OMessage *msg);
    };

    explicit OClient();
    virtual ~OClient();

    //获得唯一标识,登录后等同用户名，未登录  #IP:端口
    inline QString getSignature();

    void init();//初始化，绑定主连接的信号槽
    void addSubConn(QTcpSocket *conn);//增加次要连接，自动绑定信号槽
    void checkData(Connect *connect);//接收数据

    QString uname;//用户名
    bool isLoged;//是否已经登录

    Connect *main;//主连接
    QVector<Connect*> subConnList;//次要连接数组
    typedef QVector<Connect*>::Iterator SubConnIt;//次要连接数组迭代器简写
signals:
    void newMsgData(OClient::Connect *connect);//当已经接收到了一个(或多个)完整的数据包后发射
    //发生错误时发射
    void error(OClient::Connect *connect,QString msg,QAbstractSocket::SocketError s);
    //需要记录进日志时发射
    void log(QString msg);
private slots:
    void onData();
    void onError(QAbstractSocket::SocketError s);
};

//见OServerCore::OServerCore()中的注释
Q_DECLARE_METATYPE(OClient::Connect);

inline bool OClient::Connect::isMain()
{
    return this==client->main;
}

inline void OClient::Connect::send(OMessage *msg)
{
    conn->write(msg->exec());
}

inline QString OClient::getSignature()
{
    if(isLoged)
        return uname;
    else
        return QString("#%1:%2").arg(main->conn->peerAddress().toString()).arg(main->conn->peerPort());
}

#endif // OCLIENT_H
