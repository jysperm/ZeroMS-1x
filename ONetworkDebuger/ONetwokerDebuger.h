#ifndef ONETWOKERDEBUGER_H
#define ONETWOKERDEBUGER_H

#include <QWidget>
#include <QMap>
#include <QVector>
#include <QUdpSocket>
#include "ui_ONetwokerDebuger.h"
#include "../public/OGlobal.h"

namespace Ui
{
    class ONetwokerDebuger;
}

const QString sqlitter="--------------------";

class UdpListen
{
    //Udp监听对象，实际上也表示Udp发送对象
public:
    //是否是监听对象，只有监听对象才出现在右面的"监听/连接列表"中
    bool isListen;
    QUdpSocket *conn;//连接对象
};

class ONetwokerDebuger:public QWidget
{
    Q_OBJECT
public:
    explicit ONetwokerDebuger(QWidget *parent = 0);
    ~ONetwokerDebuger();
    inline void log(QString msg);
private:
    Ui::ONetwokerDebuger *ui;
    QMap<QString,UdpListen> udpListenList;
private slots:
    void updateList();
    void on_DoUdpListen_clicked();
    void onSocketNewData();
    void onSocketError(QAbstractSocket::SocketError s);
};

//连接对象的命名方法：
//<协议>:<远程/本地IP>:<端口>
//(协议大写TCP/UDP),TCP即为远程IP,UDP即为本地IP.

inline void ONetwokerDebuger::log(QString msg)
{
    ui->Output->append(msg);
}

inline int QBtoint(QByteArray b)
{
    //从QByteArray向int转换
    QDataStream d(b);
    int i;
    d>>i;
    return i;
}

inline QByteArray inttoQB(int i)
{
    //从int向QByteArray转换
    QByteArray b;
    QDataStream d(b);
    d<<i;
    return b;
}

#endif // ONETWOKERDEBUGER_H
