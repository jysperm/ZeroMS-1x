#ifndef OSERVERCORE_H
#define OSERVERCORE_H

#include <QObject>
#include <QTextStream>
#include <QTcpServer>
#include <QMap>
#include "OServerDataBase.h"
class OClient;

class OServerCore:public QObject
{
    Q_OBJECT
    friend class OProtocolForSC;
    friend class OClientPeer;
public:
    explicit OServerCore();
    void init();
    void start();
private:
    QString getUserStatus(QString uname);
    //用于当uname的在线状态发生变化时
    //通知所有好友列表中含有uname的用户更新用户列表(发送UserListChange消息)
    void userListChange(QString uname);
    //向标准输出打印消息，和时间，单独成行
    void log(QString msg);
    QMap<QString,OClient*> cl;
    QTextStream cin;
    QTextStream cout;
    QTcpServer server;
    OServerDataBase db;
    QMap<QString,QString> info;
private slots:
    void onNewConn();
    void onError(OClient *client);
};

#endif // OSERVERCORE_H
