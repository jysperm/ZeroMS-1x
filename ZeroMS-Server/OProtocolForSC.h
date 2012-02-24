#ifndef OPROTOCOLFORSC_H
#define OPROTOCOLFORSC_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include "OClient.h"
class QTcpSocket;

class OProtocolForSC:public QObject
{
    Q_OBJECT
public:
    explicit OProtocolForSC();
    void UserListChanged(OClient::Connect *connect,QString listname);
    void PublicKey(OClient::Connect *connect,QString publicKey);
    void LoginResult(OClient::Connect *connect,QString status,QString ip=QString());
    void Unknown(OClient::Connect *connect);
    void Info(OClient::Connect *connect,QMap<QString,QString> keys);
    void UserList(OClient::Connect *connect,QString listname,QString operation,QVector<OClient::UserlistItem> userlist);
public slots:
    void checkMsg(OClient::Connect *connect);
signals:
    void AskUserList(OClient::Connect *connect,QString listname,QString operation,bool isHasAvatar);
    void ModifyUserList(OClient::Connect *connect,QString uname,bool isAddOrRemove);
    void AskInfo(OClient::Connect *connect,QStringList keys);
    void Login(OClient::Connect *connect,QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp);
    void AskPublicKey(OClient::Connect *connect);
};

#endif // OPROTOCOLFORSC_H
