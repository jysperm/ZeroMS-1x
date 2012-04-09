#ifndef PUBLIC_OABSTRACTPEER_H
#define PUBLIC_OABSTRACTPEER_H

#include <QTcpSocket>
#include <QStringList>
#include "OMessage.h"

enum OPeerType
{
    //用于OAbstractPeer,表示连接的类型
    ServerPeer, //CS
    ClientPeer  //SC
};

class OUserlistItem
{
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
    //这是一个抽象基类，抽象一个连接，可以是SC/CS/CC
    Q_OBJECT
public:
    explicit OAbstractPeer(QTcpSocket *connect=0);
    virtual ~OAbstractPeer();
    void init();//初始化
    void collect();//回收内存

    //虚函数，返回连接的类型，派生类需要重写这个函数
    //改函数用于在checkMsg()中判定消息分拣时的角色
    virtual OPeerType getPeerType()=0;
    inline void send(OMessage *msg);//发送消息

    void UserListChanged(QString listname);
    void PublicKey(QString publicKey);
    void LoginResult(QString status,QString ip=QString());
    void Unknown();
    void Info(QMap<QString,QString> keys);
    void UserList(QString listname,QString operation,QVector<OUserlistItem> userlist);
    void ProcessError(QString errorName,QString other=QString());

    OMessage *CurrentMsg;//当前被处理的消息,用于ProcessError()

    QTcpSocket *conn;//连接对象
    QByteArray databuf;//数据缓冲
public slots:
    void checkMsg();//分拣消息
signals:
    void error(OAbstractPeer *peer,QString msg,QAbstractSocket::SocketError s);//当连接发生错误时发射

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
    debug<<item.uname<<item.status<<item.groupStatus<<item.ip<<item.avatar;
    return debug;
}

inline void OAbstractPeer::send(OMessage *msg)
{
    if(conn)
        conn->write(msg->exec());
}

#endif // PUBLIC_OABSTRACTPEER_H
