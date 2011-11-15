#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#include <QAbstractSocket>
#include <QObject>
#include "const.h"
class QApplication;
class QByteArray;
class QString;
class QTcpSocket;

class ClientCore:public QObject
{
    Q_OBJECT
public:
    explicit ClientCore(QApplication *app=0);
    virtual ~ClientCore();
    //初始化，因为有些工作不能在构造函数里完成
    virtual void init();
    //连接到
    virtual void connectTo(QString ip,int port);
    //中断连接
    virtual void abort();

    //Socket连接对象
    QTcpSocket *conn;
    //当前应用程序对象
    QApplication *app;
    //服务器时间与本地时间差值，服务器时间-本地时间
    unsigned int stime;
    //用户名
    QString uname;
    //服务器时间已经被更新
    //TODO，这个设计实在比较蛋疼，但无奈我没更好的主意了
    int uptime;
//消息发送函数:
    virtual void msgAskTime();
    virtual void msgLogin(QString username,QString pwd);
    virtual void msgAskUList();
protected:
//消息回调函数:
    virtual void msgLoginOk(QByteArray *data,unsigned int time);
    virtual void msgLoginError(QByteArray *data,unsigned int time);
    virtual void msgTime(QByteArray *data,unsigned int time);
    virtual void msgChangeUList(QByteArray *data,unsigned int time);
    virtual void msgUList(QByteArray *data,unsigned int time);

    //MD5,只是为了简写
    static QString md5(QString s);
    //从QByteArray向int转换
    static int QBtoint(QByteArray b);
    //从int向QByteArray转换
    static QByteArray inttoQB(int i);
    //抛出错误(无法理解服务器的协议)
    virtual void throwError();

    //数据缓冲
    QByteArray *databuf;
private slots:
    //收到数据，是conn发出的
    void onData();
    //错误信息，是conn发出的
    void onError(QAbstractSocket::SocketError s);
};

#endif // CLIENTCORE_H
