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
    explicit OAbstractPeer(OPeerType peerType,QTcpSocket *connect=0);
    virtual ~OAbstractPeer();
    //初始化,需要在设置conn后调用
    void init();
    void collect();//回收内存

    inline OPeerType getPeerType();//返回连接类型，即peerType
    inline void send(OMessage *msg);//发送消息

    //下面是用来发送消息的函数

    void Login(QString uname,QString pwdHash,QVector<int> p2pPort=QVector<int>(),bool isMain=true,bool isForce=false,bool isShowIp=true);
    void LoginResult(QString status,QString ip=QString());
    void Info(const QMap<QString, QString> &values);
    void AskPublicKey();
    void PublicKey(QString publicKey);
    void Unknown();
    void UserListChanged(QString listname);
    void AskUserList(QString listname,QString operation,bool isHasAvatar);
    void UserList(QString listname,QString operation,QVector<OUserlistItem> userlist);
    void SystemMsg(QString message);
    void NewMsg(int id,QString view,QString uname, QString message);
    void NewRequest(int id,QString uname,QString invitation,QString message);
    void UserInfo(QString uname, QMap<QString, QString> values);
    void ProcessError(QString errorName,QString other=QString());
    void NewGroupRequest(int id,QString uname,QString requester,QString message);

    //当前被处理的消息,在checkMsg()中被设置,用于ProcessError()
    OMessage *currentMsg;

    QTcpSocket *conn;//连接对象
    QByteArray databuf;//数据缓冲
    const OPeerType peerType;
public slots:
    void checkMsg();//分拣消息
signals:
    void error(OAbstractPeer *peer,QString msg,QAbstractSocket::SocketError s);//当连接发生错误时发射

    //下面这些信号会在收到对应的消息时发射

    void onLogin(QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp);
    void onLoginResult(QString status,QString ip);
    void onAskInfo(QStringList keys);
    void onAskPublicKey();
    void onPublicKey(QString publicKey);
    void onUserListChanged(QString listname);
    void onAskUserList(QString listname,QString operation,bool isHasAvatar);
    void onUserList(QString listname,QString operation,QVector<OUserlistItem> userlist);
    void onModifyUserList(QString listname,QString uname,QString operation,QString message);
    void onState(QString status);
    void onLogout();
    void onSendMsg(QString uname,QString message);
    void onUserRequest(QString uname,QString message);
    void onRequestResult(int id,QString result);
    void onModifyGroup(QString group,QString uname,QStringList operators);
    void onAskUserInfo(QString uname,QStringList keys);
    void onOK(QString id);
    void onModifyInfo(QString uname,QMap<QString,QString> values);
protected slots:
    void onError(QAbstractSocket::SocketError s=QAbstractSocket::UnknownSocketError);//用于与conn的error()信号绑定
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

inline OPeerType OAbstractPeer::getPeerType()
{
    return peerType;
}

inline void OAbstractPeer::send(OMessage *msg)
{
    if(conn)
        conn->write(msg->exec());
}

#endif // PUBLIC_OABSTRACTPEER_H
