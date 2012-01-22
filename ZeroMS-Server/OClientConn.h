#ifndef OCLIENTCONN_H
#define OCLIENTCONN_H

#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <QVector>
#include "global.h"

class OClientConn:public QObject
{
    Q_OBJECT
public:
    class SubConn
    {
    public:
        SubConn(QTcpSocket *conn):conn(conn){}
        QTcpSocket *conn;//连接对象
        QByteArray databuf;//数据缓冲
    };

    explicit OClientConn();
    virtual ~OClientConn();
    inline QString getSignature();//获得唯一标识
    void checkData(QTcpSocket *conn,QByteArray *databuf);//接收数据
    void init();//初始化，绑定主连接的信号槽
    void addSubConn(QTcpSocket *conn);//增加次要连接，自动绑定信号槽

    QString uname;//用户名
    bool isLoged;//是否登录
    QTcpSocket *conn;//连接对象
    QByteArray databuf;//数据缓冲
    QVector<SubConn*> subConnList;//次要连接数组
    typedef QVector<SubConn*>::Iterator SubConnIt;
signals:
    void newMsgData(QString uname,QTcpSocket *conn,QByteArray *databuf);
    void error(QString uname,QString msg,QAbstractSocket::SocketError s,bool isMain);
    void log(QString msg);
private slots:
    void onData();
    void onError(QAbstractSocket::SocketError s);
};

inline QString OClientConn::getSignature()
{
    if(isLoged)
        return uname;
    else
        return QString("#%1:%2").arg(conn->peerAddress().toString()).arg(conn->peerPort());
}

#endif // OCLIENTCONN_H
