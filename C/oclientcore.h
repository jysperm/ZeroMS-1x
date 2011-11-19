#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#include <QAbstractSocket>
#include <QObject>
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
    //连接到服务器
    virtual void connectTo(QString ip,int port);
    //中断连接
    virtual void abort();

    //Socket连接对象
    QTcpSocket *conn;
    //服务器时间与本地时间差值，服务器时间-本地时间
    unsigned int timediff;
    //用户名
    QString uname;
//消息发送函数:
    virtual void msgAskTime();
    virtual void msgPing();
    virtual void msgExit();
    virtual void msgCMsg();
    virtual void msgLogin(QString username,QString pwd);
    virtual void msgAskUList();
protected:
//消息回调函数:
    virtual void msgError(QByteArray *data,unsigned int time);
    virtual void msgSMsg(QByteArray *data,unsigned int time);
    virtual void msgTime(QByteArray *data,unsigned int time);
    virtual void msgLoginOk(QByteArray *data,unsigned int time);
    virtual void msgLoginError(QByteArray *data,unsigned int time);
    virtual void msgUList(QByteArray *data,unsigned int time);
    virtual void msgChangeUList(QByteArray *data,unsigned int time);

    //数据缓冲
    QByteArray *databuf;
signals:
    void onError(QByteArray *data,unsigned int time);
    void onSMsg(QByteArray *data,unsigned int time);
    void onTime(QByteArray *data,unsigned int time);
    void onLoginOk(QByteArray *data,unsigned int time);
    void onLoginError(QByteArray *data,unsigned int time);
    void onUList(QByteArray *data,unsigned int time);
    void onChangeUList(QByteArray *data,unsigned int time);
    void onData();//收到数据
    void onSocketError(QAbstractSocket::SocketError s);//遇到错误
    void onConnected();//已连接到服务器
    void onInit();//当init函数被调用时，该信号被发射，可以用来初始化一些数据
    void onAborted();//断开连接
    void onTimeChange();//服务器时间被更新
private slots:
    //收到数据，是conn发出的，该函数还会进行消息分发
    void dataCome();
    //错误信息，是conn发出的
    void socketError(QAbstractSocket::SocketError s);
};

#endif // CLIENTCORE_H
