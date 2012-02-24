#ifndef OSERVERCORE_H
#define OSERVERCORE_H

#include <QObject>
#include <QTextStream>
#include <QTcpServer>
#include <QStringList>
#include "ODataBase.h"
#include "../public/OSettings.h"
#include "global.h"
#include "OClient.h"
#include "OProtocolForSC.h"

class OServerCore:public QObject
{
    Q_OBJECT
    friend class OProtocolForSC;
public:
    explicit OServerCore();
    void init();
    void start();
private:
    QString getUserStatus(QString uname);
    void log(QString msg);
    QMap<QString,OClient*> cl;
    QTextStream cin;
    QTextStream cout;
    QTcpServer server;
    OProtocolForSC protocol;
    ODataBase db;
    QMap<QString,QString> info;
private slots:
    void AskUserList(OClient::Connect *connect,QString listname,QString operation,bool isHasAvatar);
    void ModifyUserList(OClient::Connect *connect,QString uname,bool isAddOrRemove);
    void AskInfo(OClient::Connect *connect,QStringList keys);
    void Login(OClient::Connect *connect,QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp);
    void AskPublicKey(OClient::Connect *connect);
    void onNewConn();
    void onError(OClient::Connect *connect,QString msg,QAbstractSocket::SocketError s);
};

#endif // OSERVERCORE_H
