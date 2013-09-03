#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#include "const.h"
#include "../public/OPacket.h"


class OClientCore:public QObject
{
    Q_OBJECT
public:
    enum ErrorType
    {
        //解释请参见errorString()中的注释
        //TODO,其实这里用Q_ENUM好些，只不过找不到中文的相关资料
        Unknown=-1,
        NoError,
        CantUnderstand,
        MsgError,
        SocketCantConnect,
        SocketConnectionAbort,
        SocketOthers
    };

    explicit OClientCore();
    virtual ~OClientCore();

    virtual void init();//初始化
    virtual void connectTo(QString ip,int port);//连接到服务器
    virtual void abort();//中断连接
    virtual QString errorString(ErrorType e=(ErrorType)-2);//获得错误信息的文本描述

    //几个工具函数
    inline static int QBtoint(QByteArray b);//从QByteArray向int转换
    inline static QByteArray inttoQB(int i);//从int向QByteArray转换
    inline static QString md5(QString s);//简写MD5操作

    inline void pingUpdate();//更新上次发送消息的时间

    QTcpSocket *conn;//Socket连接对象
    //服务器时间与本地时间差值，服务器时间-本地时间
    unsigned int timeDiff;
    unsigned int lastMsgTime;//上次发送消息的时间，用于计算何时向服务器发送相应消息
    QString myname;//用户名
    bool isLoged;//是否已经登陆
    ErrorType lastError;
    int timeOffLine;//客户端多长时间向服务器发送在线相应消息

public slots:
    //消息发送函数:
    virtual void msgAskTime();
    virtual void msgPing();
    virtual void msgExit();
    virtual void msgCMsg(QString objname,QString msg);
    virtual void msgLogin(QString uname,QString pwd);
    virtual void msgAskUList();
protected:
//可重载消息回调函数:
    //对于有的消息，如果选择重载，需要自己解析数据包，而信号中发射的是已经解析后的消息
    virtual void msgSMsg(OPacket &packet);
    virtual void msgLoginOk(OPacket &packet);
    virtual void msgLoginError(OPacket &packet);
    virtual void msgUList(OPacket &packet);

    //发送消息
    virtual void sendPacket(OPacket &packet);
    //收到消息(该函数内会对消息进行分拣，交给相应的处理函数，派生类可以重载这个函数来拦截消息)
    virtual void receivePacket(OPacket &packet);

    QByteArray *databuf;//数据缓冲
protected slots:
    //默认的错误处理函数，如果发生Socket连接错误会断开连接;如不希望断开连接请重载
    virtual void Error(OClientCore::ErrorType e,QString msg,QAbstractSocket::SocketError s);
signals:
    void onSMsg(QString user,QString view,QString msg);
    void onLoginOk();
    void onLoginError();
    void onUList(QStringList &users);
    void onChangeUList();
    void onData();//收到数据
    void onError(OClientCore::ErrorType e,QString msg,QAbstractSocket::SocketError s);//遇到错误
    void onConnected();//已连接到服务器
    void onInit();//当init函数被调用时，该信号被发射，可以用来初始化一些数据
    void onAborted();//断开连接
    void onTimeChange(unsigned int time);//服务器时间被更新
private:
//不可重载消息回调函数:
    virtual void msgTime(OPacket &packet);
    virtual void msgChangeUList(OPacket &packet);
    //定时器，用于定时与服务器保持相应
    QTimer *pingTimer;
private slots:
    //pingTimer的槽
    void pingTimeOut();
    //收到数据，是conn发出的
    void dataCome();
    //Socket错误，是conn发出的
    void socketError(QAbstractSocket::SocketError s);
};

inline int OClientCore::QBtoint(QByteArray b)
{
    QDataStream d(b);
    int i;
    d>>i;
    return i;
}

inline QByteArray OClientCore::inttoQB(int i)
{
    QByteArray b;
    QDataStream d(b);
    d<<i;
    return b;
}

inline QString OClientCore::md5(QString s)
{
    return QString(QCryptographicHash::hash(QByteArray().append(s), QCryptographicHash::Md5).toHex());
}

inline void OClientCore::pingUpdate()
{
    lastMsgTime=QDateTime::currentDateTime().toTime_t();
}

#endif // CLIENTCORE_H
