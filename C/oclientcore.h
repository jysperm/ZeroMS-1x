#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#include <QAbstractSocket>
#include <QObject>
#include <QStringList>
#include "const.h"
class QByteArray;
class QString;
class QTcpSocket;

class OClientCore:public QObject
{
    Q_OBJECT
public:
    explicit OClientCore();
    virtual ~OClientCore();
    //初始化，因为有些工作不能在构造函数里完成（好像在构造函数里没法往自己身上绑定信号槽）
    virtual void init();
    virtual void connectTo(QString ip,int port);//连接到服务器
    virtual void abort();//中断连接


    QTcpSocket *conn;//Socket连接对象
    //服务器时间与本地时间差值，服务器时间-本地时间
    unsigned int timeDiff;
    QString myname;//用户名
    int isLoged;//是否已经登陆
//消息发送函数:
    virtual void msgAskTime();
    virtual void msgPing();
    virtual void msgExit();
    virtual void msgCMsg(QString objname,QString msg);
    virtual void msgLogin(QString uname,QString pwd);
    virtual void msgAskUList();
protected:
//可重载消息回调函数:
    virtual void msgSMsg(QByteArray *data,unsigned int time);
    virtual void msgLoginOk(QByteArray *data,unsigned int time);
    virtual void msgLoginError(QByteArray *data,unsigned int time);
    //对于这个消息，如果选择重载，需要自己解析数据包，而信号中发射的是解析后的QStringList
    virtual void msgUList(QByteArray *data,unsigned int time);

    QByteArray *databuf;//数据缓冲
signals:
    void onSMsg(QString objName,QString from,QString uname,QString msg);
    void onLoginOk();
    void onLoginError();
    void onUList(QStringList &users);
    void onChangeUList();
    void onData();//收到数据
    void onError(QString msg);//遇到错误
    void onConnected();//已连接到服务器
    void onInit();//当init函数被调用时，该信号被发射，可以用来初始化一些数据
    void onAborted();//断开连接
    void onTimeChange(unsigned int time);//服务器时间被更新
private:
//不可重载消息回调函数:
    virtual void msgError(QByteArray *data,unsigned int time);
    virtual void msgTime(QByteArray *data,unsigned int time);
    virtual void msgChangeUList(QByteArray *data,unsigned int time);
    //发生错误(会被socketError调用)
    //该函数中会断开连接
    virtual void Error(QString msg="");
private slots:
    //收到数据，是conn发出的，该函数还会进行消息分发
    void dataCome();
    //Socket错误，是conn发出的
    void socketError(QAbstractSocket::SocketError s);
};

#endif // CLIENTCORE_H
