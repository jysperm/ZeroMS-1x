#ifndef OSERVERPEER_H
#define OSERVERPEER_H

#include "../public/OAbstractPeer.h"
#include "global.h"

class OServerPeer:public OAbstractPeer
{
    Q_OBJECT
    //资源类
    //该类继承OAbstractPeer,实现一个与服务器连接的连接对象
public:
    explicit OServerPeer(QTcpSocket *connect=0);
    void init();
public slots:
    void onLoginResult(QString status,QString ip);
    void onPublicKey(QString publicKey);
    void onUserListChanged(QString listname);
    void onUserList(QString listname,QString operation,const QVector<OUserlistItem> &userlist);
};

#endif // OSERVERPEER_H
