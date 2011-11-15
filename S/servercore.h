#ifndef SERVERCORE_H
#define SERVERCORE_H

#include <QTcpServer>
#include <QDateTime>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCryptographicHash>
#include "const.h"
#include "socketlist.h"
#include "clientconn.h"

class ServerCore:public QTcpServer
{
    Q_OBJECT
    //该类是通讯核心，也是整个程序的主体，main()函数负责启动它
    //每一项动作都会打印日志(初步是向标准输出打印日志)
public:
    ServerCore();
    //开始监听
    void run();
    //终止监听
    void stop();
private:
    SocketList sl;
    //下面两个对象是用来post网页来实现登陆的
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    //打印日志
    void log(QString);
    //从QByteArray向int转换
    static int QBtoint(QByteArray);
    //从int向QByteArray转换
    static QByteArray inttoQB(int);
    //MD5,为了简写
    static QString md5(QString);
    //分发消息
    void checkMsg(QString);
    //向客户端抛出错误
    void throwError(QString);
    //关闭到客户端的连接
    void closeClient(QString);

    void msgLogin(QString,QByteArray*,unsigned int);
    void msgChangeUList();
    void msgUList(QString);
    void msgAskUList(QString,QByteArray*,unsigned int);
    void msgAckTime(QString,QByteArray*,unsigned int);
    void msgTime(QString);
    void msgLoginOk(QString);
    void msgLoginError(QString);
private slots:
    //登录结果,是reply对象发出的
    void LoginResult();
    //收到新连接,是this类发出的
    void newConn();
    //收到数据，是sl中各连接对象发出的
    void onData();
    //错误信息，是sl中各连接对象发出的
    void onError(QAbstractSocket::SocketError);
};

#endif // SERVERCORE_H
