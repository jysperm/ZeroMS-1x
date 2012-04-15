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
    //值类
    //是整个服务器的核心，运行时仅存在一个实例
    Q_OBJECT
public:
    explicit OServerCore();
    void init();
    void start();

    QString getUserStatus(QString uname);
    //用于当uname的在线状态发生变化时
    //通知所有好友列表中含有uname的用户更新用户列表(发送UserListChange消息)
    //uname可能为小组，这时表示小组的成员列表发生变化，通知所有成员
    void userListChange(QString uname);
    //处理对应的请求,参数被省略时则尝试处理所有请求
    void processRequest(int id=-1);

    QMap<QString,OClient*> cl;
    QTextStream cin;
    QTextStream cout;
    OServerDataBase db;
    QMap<QString,QString> info;
    unsigned int upTime;
public slots:
    //向标准输出打印消息，和时间，单独成行
    void log(QString msg=QString());
private:
    QTcpServer server;
private slots:
    void onNewConn();
    void onError(OClient *client);
};

#endif // OSERVERCORE_H
