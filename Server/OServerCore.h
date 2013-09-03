#ifndef OSERVERCORE_H
#define OSERVERCORE_H

#include "const.h"
#include "../public/OPacket.h"
#include "../public/OSettings.h"
class OClient;

class OServerCore:public QTcpServer
{
    //该类是通讯核心，也是整个程序的主体，main()函数负责启动它
    Q_OBJECT
public:
    explicit OServerCore();
    virtual ~OServerCore();
    void run();			//开始监听
    void stop();		//终止监听
private:
    //打印日志
    void log(QString msg);
    //分发消息
    void checkMsg(QString uname);

    //几个工具函数
    inline static int QBtoint(QByteArray b);//从QByteArray向int转换
    inline static QByteArray inttoQB(int i);//从int向QByteArray转换
    inline static QString md5(QString s);//简写MD5操作

    void msgError(QString uname);
    void msgAskTime(QString uname,QByteArray *data,unsigned int time);
    void msgTime(QString uname);
    void msgPing(QString uname,QByteArray *data,unsigned int time);
    void msgExit(QString uname,QByteArray *data,unsigned int time);
    void msgCMsg(QString uname,QByteArray *data,unsigned int time);
    void msgSMsg(QString user,QString view,QString uname,QString msg);
    void msgLogin(QString uname,QByteArray *data,unsigned int time,QString API);
    void msgLoginOk(QString);
    void msgLoginError(QString);
    void msgAskUList(QString uname,QByteArray *data,unsigned int time);
    void msgUList(QString);
    void msgChangeUList(QStringList users);

    //客户的连接数组
    QMap<QString,OClient*> cl;
    //因为如果写在const.h中需要包含QMap，代价太大了，所以写在这里
    typedef QMap<QString,OClient*>::iterator it;
    //下面两个对象是用来post网页来实现登陆的
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    //定时器，用于定时剔除长时间未相应的用户
    QTimer *timer;
    //配置文件读取器
    OSettings config;
    //日志文件指针
    QFile *logFile;
private slots:
    //timer的槽
    void checkTimeOut();
    //登录结果,是reply对象发出的
    void LoginResult();
    //收到新连接,是this类的基类QTcpServer发出的
    void onNewConn();
    //收到数据，是cl中各连接对象发出的
    void onData();
    //错误信息，是cl中各连接对象发出的
    void onError(QAbstractSocket::SocketError s);
};

inline int OServerCore::QBtoint(QByteArray b)
{
    QDataStream d(b);
    int i;
    d>>i;
    return i;
}

inline QByteArray OServerCore::inttoQB(int i)
{
    QByteArray b;
    QDataStream d(b);
    d<<i;
    return b;
}

inline QString OServerCore::md5(QString s)
{
    return QString(QCryptographicHash::hash(QByteArray().append(s), QCryptographicHash::Md5).toHex());
}

#endif // OSERVERCORE_H
