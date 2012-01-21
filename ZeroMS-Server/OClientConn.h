#ifndef OCLIENTCONN_H
#define OCLIENTCONN_H

#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <QVector>

class OClientConn:public QObject
{
    Q_OBJECT
public:
    explicit OClientConn();
    inline QString getSignature();//获得唯一标识

    QString uname;//用户名
    bool isLoged;//是否登录
    QTcpSocket *conn;//连接对象
    QByteArray *databuf;//数据缓冲
    QVector subConnList;//次要连接数组
private slots:
    void onData();
};

inline QString OClientConn::getSignature()
{
    if(isLoged)
        return uname;
    else
        return QString("#%1:%2").arg(conn->peerAddress().toString()).arg(conn->peerPort());
}

#endif // OCLIENTCONN_H
