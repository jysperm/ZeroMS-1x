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
    void PublicKey(OClient::Connect *connect,QString publicKey);
    void LoginResult(OClient::Connect *connect,QString status,QHostAddress ip=QHostAddress());
    void Info(OClient::Connect *connect,QMap<QString,QString> keys);
public slots:
    void checkMsg(OClient::Connect *connect);
signals:
    void AskInfo(OClient::Connect *connect,QStringList keys);
    void Login(OClient::Connect *connect,QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp);
    void AskPublicKey(OClient::Connect *connect);
};

#endif // OPROTOCOLFORSC_H
