#ifndef PUBLIC_OABSTRACTPEER_H
#define PUBLIC_OABSTRACTPEER_H

#include <QTcpSocket>
#include <QStringList>
#include "OMessage.h"

enum OPeerType
{
    //用于OAbstractPeer,表示连接的类型
    ServerPeer, //CS
    P2PPeer,    //CC
    ClientPeer  //SC
};

class OUserlistItem
{
    //值类
    //抽象一个好友列表条目，见 http://0-ms.org/wiki/show/UserList
public:
    inline bool operator==(const OUserlistItem other) const;

    QString uname;
    QString status;
    QString groupStatus;
    QString ip;
    QVector<int> p2pPorts;
    QString avatar;
};

class OAbstractPeer:public QObject
{
    //资源类
    //这是一个抽象基类，抽象一个连接，可以是SC/CS/CC
    Q_OBJECT
public:
    explicit OAbstractPeer(QTcpSocket *connect=0);
    virtual ~OAbstractPeer();
    //初始化,需要在设置conn后调用
    void init();
    void collect();//回收内存

    //虚函数，返回连接的类型(OPeerType型枚举)，派生类需要重写这个函数
    //该函数用于在checkMsg()中判定消息分拣时的角色
    virtual OPeerType getPeerType()=0;
    inline void send(OMessage *msg);//发送消息

    //下面是用来发送消息的函数

    void LoginResult(QString status,QString ip=QString());
    void Info(QMap<QString,QString> keys);
    void PublicKey(QString publicKey);
    void Unknown();
    void UserListChanged(QString listname);
    void UserList(QString listname,QString operation,QVector<OUserlistItem> userlist);
    void ProcessError(QString errorName,QString other=QString());

    //当前被处理的消息,在checkMsg()中被设置,用于ProcessError()
    OMessage *CurrentMsg;

    QTcpSocket *conn;//连接对象
    QByteArray databuf;//数据缓冲
public slots:
    void checkMsg();//分拣消息
signals:
    void error(OAbstractPeer *peer,QString msg,QAbstractSocket::SocketError s);//当连接发生错误时发射

    //下面这些信号会在收到对应的消息时发射

    void AskUserList(QString listname,QString operation,bool isHasAvatar);
    void ModifyUserList(QString listname,QString uname,QString operation,QString message);
    void AskInfo(QStringList keys);
    void Login(QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp);
    void AskPublicKey();
    void State(QString status);
private slots:
    void onError(QAbstractSocket::SocketError s);//用于与conn的error()信号绑定
};

inline bool OUserlistItem::operator==(const OUserlistItem other) const
{
    return (this->uname==other.uname &&
           this->status==other.status &&
           this->groupStatus==other.groupStatus &&
           this->ip==other.ip &&
           this->p2pPorts==other.p2pPorts &&
           this->avatar==other.avatar);
}

inline QDebug &operator<<(QDebug &debug, const OUserlistItem &item)
{
    //用于调试
    debug<<item.uname<<item.status<<item.groupStatus<<item.ip<<item.avatar;
    return debug;
}

inline void OAbstractPeer::send(OMessage *msg)
{
    if(conn)
        conn->write(msg->exec());
}

#endif // PUBLIC_OABSTRACTPEER_H
