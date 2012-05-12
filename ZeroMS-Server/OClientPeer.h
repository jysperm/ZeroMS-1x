#ifndef OCLIENTPEER_H
#define OCLIENTPEER_H

#include "../public/OAbstractPeer.h"

class OClient;

class OClientPeer:public OAbstractPeer
{
    //资源类
    //该类继承OAbstractPeer,实现一个与客户端连接的连接对象
    Q_OBJECT
public:
    explicit OClientPeer(QTcpSocket *connect=0);
    void init();

    //是否是client的主连接
    //注意，因为循环依赖的问题，该函数的定义被移动到了OClient.h中.
    inline bool isMain();

    OClient *client;//所属的OClient对象
public slots:
    //下面这些槽函数被绑定到OAbstractPeer的相应信号
    //用于处理相应的消息

    void onLogin(QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp);
    void onAskInfo(QStringList keys);
    void onAskPublicKey();
    void onAskUserList(QString listname,QString operation,bool isHasAvatar);
    void onModifyUserList(QString listname,QString uname,QString operation,QString message);
    void onState(QString status);
    void onUserRequest(QString uname,QString message);
    void onRequestResult(int id,QString result);
    void onModifyGroup(QString group,QString uname,QStringList operators);
    void onAskUserInfo(QString uname,QStringList keys);
    void onModifyInfo(QString uname,QMap<QString,QString> values);
private:
    QString publicKey;//公钥
};

#endif // OCLIENTPEER_H
