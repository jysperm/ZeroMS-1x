#ifndef OCLIENTPEER_H
#define OCLIENTPEER_H

#include "../public/OAbstractPeer.h"

class OClient;

class OClientPeer:public OAbstractPeer
{
    Q_OBJECT
public:
    explicit OClientPeer(QTcpSocket *connect=0);

    inline bool isMain();
    OPeerType getPeerType();

    OClient *client;
    QString publicKey;//公钥
public slots:
    void onLogin(QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp);
};

#endif // OCLIENTPEER_H
