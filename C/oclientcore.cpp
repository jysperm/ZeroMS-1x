#include <QAbstractSocket>
#include <QApplication>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDataStream>
#include <QDateTime>
#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include "const.h"
#include "inline.h"
#include "oclientcore.h"

//public:
OClientCore::OClientCore():conn(0),timediff(0),databuf(0)
{

}

OClientCore::~OClientCore()
{
    DELETE(conn);
    DELETE(databuf);
}

void OClientCore::init()
{
    emit onInit();
}

void OClientCore::connectTo(QString ip,int port)
{
    conn=new QTcpSocket;
    databuf=new QByteArray;
    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
    connect(conn,SIGNAL(readyRead()),this,SLOT(dataCome()));
    connect(conn,SIGNAL(connected()),this,SLOT(onConnected()));
    conn->connectToHost(QHostAddress(ip),port);
}

void OClientCore::abort()
{
    conn->abort();
    conn->waitForDisconnected();
    DELETE(conn);
    DELETE(databuf);
    emit onAborted();
}

//消息发送函数:
void OClientCore::msgAskTime()
{

}

void OClientCore::msgPing()
{

}

void OClientCore::msgExit()
{

}

void OClientCore::msgCMsg()
{

}

void OClientCore::msgLogin(QString username,QString pwd)
{

}

void OClientCore::msgAskUList()
{

}

//protected:
//消息回调函数:
void OClientCore::msgError(QByteArray *data,unsigned int time)
{

}

void OClientCore::msgSMsg(QByteArray *data,unsigned int time)
{

}

void OClientCore::msgTime(QByteArray *data,unsigned int time)
{

}

void OClientCore::msgLoginOk(QByteArray *data,unsigned int time)
{

}

void OClientCore::msgLoginError(QByteArray *data,unsigned int time)
{

}

void OClientCore::msgUList(QByteArray*,unsigned int time)
{

}

void OClientCore::msgChangeUList(QByteArray*,unsigned int time)
{

}

void OClientCore::Error(QString msg)
{

}

//private slots:
void OClientCore::dataCome()
{
    databuf->append(conn->readAll());
    if(databuf->size()<P_HEADLEN)
        return;
    int ver=QBtoint(databuf->mid(0,4));
    int len=QBtoint(databuf->mid(4,4));
    if(!checkVer(ver))
    {
        Error(tr("不支持的协议版本，可能是客户端已经过期"));
        return;
    }
    if(databuf->size()>=(len+P_HEADLEN))
    {
        //如果已经接收到了数据包的全部数据，进行分发命令
        int type=QBtoint(databuf->mid(8,4));
        unsigned int time=QBtoint(databuf->mid(12,4));
        QByteArray *msgData=new QByteArray(databuf->mid(P_HEADLEN,len));
        switch(type)
        {
            case M_Error:
                msgError(msgData,time);
            case M_SMsg:
                msgSMsg(msgData,time);
            case M_Time:
                msgTime(msgData,time);
            case M_LoginOk:
                msgLoginOk(msgData,time);
            case M_LoginError:
                msgLoginError(msgData,time);
            case M_UList:
                msgUList(msgData,time);
            case M_ChangeUList:
                msgChangeUList(msgData,time);
            default:
                Error(tr("不支持的协议版本，可能是客户端已经过期"));
        }
        DELETE(msgData);
        if(databuf)
            databuf->remove(0,P_HEADLEN+len);
    }
}

void OClientCore::socketError(QAbstractSocket::SocketError s)
{
    //throwError();
}
