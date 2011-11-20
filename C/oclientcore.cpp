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

//private slots:
void OClientCore::dataCome()
{
    //分发消息，该函数将根据消息的类型分发给具体的处理函数
    databuf->append(conn->readAll());
    if(databuf->size() < P_HEADLEN)
	return;
    //取前32位(协议版本)，注意只是复制，并没有从data中删除
    unsigned int ver=QBtoint(databuf->mid(0,4));
    unsigned int len=QBtoint(databuf->mid(4,4));
    if(!checkVer(ver))
    {
        //throwError();
	return;
    }
    //这是因为包头中的长度字段并不包含头部本身的长度
    if(databuf->size() >= (P_HEADLEN+len))
    {
	//如果已经接收到了全部数据，进行分发命令
	unsigned int type=QBtoint(databuf->mid(8,4));
	unsigned int time=QBtoint(databuf->mid(12,4));
	unsigned int len=QBtoint(databuf->mid(4,4));
        QByteArray *msgData=new QByteArray(databuf->mid(P_HEADLEN,len));
	switch(type)
	{
            case M_Time:
		msgTime(msgData,time);
		break;
            case M_LoginOk:
		msgLoginOk(msgData,time);
		break;
            case M_LoginError:
		msgLoginError(msgData,time);
		break;
            case M_ChangeUList:
		msgChangeUList(msgData,time);
		break;
            case M_UList:
		msgUList(msgData,time);
		break;
	    default:
                //throwError();
		return;
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
