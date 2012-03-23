#ifndef PUBLIC_OABSTRACTPEER_H
#define PUBLIC_OABSTRACTPEER_H

#include <QObject>
#include <QTcpSocket>
#include "OMessage.h"
#include "OGlobal.h"

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

    void UserListChanged(QString listname);
    void PublicKey(QString publicKey);
    void LoginResult(QString status,QString ip=QString());
    void Unknown();
    void Info(QMap<QString,QString> keys);
    void UserList(QString listname,QString operation,QVector<OUserlistItem> userlist);
public slots:
    void checkMsg();
signals:
    void AskUserList(QString listname,QString operation,bool isHasAvatar);
    void ModifyUserList(QString listname,QString uname,bool isAddOrRemove,QString message);
    void AskInfo(QStringList keys);
    void Login(QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp);
    void AskPublicKey();
    void State(QString status);
protected:
    inline void send(OMessage *msg);
    QTcpSocket *conn;//连接对象
    QByteArray databuf;//数据缓冲
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
