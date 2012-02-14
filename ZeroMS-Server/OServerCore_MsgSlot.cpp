#include "OServerCore.h"

//private slots:
void OServerCore::Login(OClient::Connect *connect,QString uname,QString pwdHash,bool isMain,bool isForce,bool isShowIp)
{
    log("登陆");
    log(QString("%1 %2 %3 %4 %5").arg(uname).arg(pwdHash).arg(isMain).arg(isForce).arg(isShowIp));
}

void OServerCore::AskPublicKey(OClient::Connect *connect)
{
    protocol.PublicKey(connect);
    log(QString("%1 请求公钥").arg(connect->client->getSignature()));
}
