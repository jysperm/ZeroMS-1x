#ifndef PUBLIC_OABSTRACTPEER_H
#define PUBLIC_OABSTRACTPEER_H

#include <QObject>
#include <QTcpSocket>
#include "OMessage.h"
#include "OGlobal.h"

enum OPeerType
{
   ServerPeer,ClientPeer
};

class OUserlistItem
{
public:
    inline bool operator==(const UserlistItem other) const;

    QString uname;
    QString status;
    QString groupStatus;
    QString ip;
    QVector<int> p2pPorts;
    QString avatar;
};

class OAbstractPeer:public QObject
{
    Q_OBJECT
public:
    explicit OAbstractPeer(QTcpSocket *connect=0);
    virtual ~OAbstractPeer();
    void init();//初始化
    void collect();

    virtual OPeerType getPeerType()=0;
    inline void send(OMessage *msg);

    void SCUserListChanged(QString listname);
    void SCPublicKey(QString publicKey);
    void SCLoginResult(QString status,QString ip=QString());
    void SCUnknown();
    void SCInfo(QMap<QString,QString> keys);
    void SCUserList(QString listname,QString operation,QVector<OUserlistItem> userlist);
public slots:
    void checkMsg();
signals:
    void error(OAbstractPeer *peer,QString msg,QAbstractSocket::SocketError s);

    void CSAskUserList(QString listname,QString operation,bool isHasAvatar);
    void CSModifyUserList(QString listname,QString uname,bool isAddOrRemove,QString message);
    void CSAskInfo(QStringList keys);
    void CSLogin(QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp);
    void CSAskPublicKey();
    void CSState(QString status);
protected:
    QTcpSocket *conn;//连接对象
    QByteArray databuf;//数据缓冲
private slots:
    void onError(QAbstractSocket::SocketError s);
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

inline void OAbstractPeer::send(OMessage *msg)
{
    if(conn)
        conn->write(msg->exec());
}

#endif // PUBLIC_OABSTRACTPEER_H
