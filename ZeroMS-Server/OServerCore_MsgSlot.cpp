#include "OServerCore.h"

//private slots:
void OServerCore::Login(OClient::Connect *connect,QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp)
{
    log("登陆");
    log(QString("%1 %2 %3 %4 %5").arg(uname).arg(pwdHash).arg(isMain).arg(isForce).arg(isShowIp));

    qDebug()<<db.checkPWD(uname,pwdHash,connect->publicKey);
}

void OServerCore::AskPublicKey(OClient::Connect *connect)
{
    QByteArray key;

    //这里的公钥采用15个ascii从32到126的随机字符
    for(int i=0;i<15;i++)
    {
        char c = ( qrand()%(126-32) )+32;
        key.append(QString(c));
    }
    connect->publicKey=QString(key);

    protocol.PublicKey(connect,key);
    log(QString("%1 请求公钥").arg(connect->client->getSignature()));
}
