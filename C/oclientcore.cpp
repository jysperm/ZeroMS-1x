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
OClientCore::OClientCore():conn(0),stime(0),uptime(0),databuf(0)
{

}

OClientCore::~OClientCore()
{
    DELETE(conn);
    DELETE(databuf);
}

void OClientCore::init()
{

}

void OClientCore::connectTo(QString ip,int port)
{
    conn=new QTcpSocket;
    databuf=new QByteArray;
    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    connect(conn,SIGNAL(readyRead()),this,SLOT(onData()));
    conn->connectToHost(QHostAddress(ip),port);
}

void OClientCore::abort()
{

}

//消息发送函数:
void OClientCore::msgAskTime()
{
    QByteArray data;
    QByteArray msgData;
    QDataStream DSdata(&data,QIODevice::ReadWrite);
    unsigned int time=QDateTime::currentDateTime().toTime_t();

    DSdata<<P_VER<<msgData.size()<<M_AskTime<<time;
    data.append(msgData);

    conn->write(data);
}

void OClientCore::msgLogin(QString username,QString pwd)
{
    unsigned int ltime=QDateTime::currentDateTime().toTime_t();
    unsigned int sertime=ltime+stime;
    //下面这句就是等到离下次密码更新时间大于5秒的时候
    //....总之很绕口，大家看一下通讯协议
    while((sertime%(unsigned int)10)>5)
    {
        qApp->processEvents();
	ltime=QDateTime::currentDateTime().toTime_t();
	sertime=ltime+stime;
    }

    unsigned int time=sertime-(sertime%10);
    QString spwd=md5(md5(QString::number(time))+md5(username)+md5(pwd));

    QByteArray data;
    QByteArray msgData;
    QDataStream DSdata(&data,QIODevice::ReadWrite);
    ltime=QDateTime::currentDateTime().toTime_t();

    msgData.append(username+" "+spwd+" "+QString::number(CLIENT_VER_NUM)+" "+CLIENT_NAME);

    DSdata<<P_VER<<msgData.size()<<M_Login<<time;
    data.append(msgData);
    this->uname=username;
    conn->write(data);
}

void OClientCore::msgAskUList()
{
    QByteArray data;
    QByteArray msgData;
    QDataStream DSdata(&data,QIODevice::ReadWrite);
    unsigned int time=QDateTime::currentDateTime().toTime_t();

    DSdata<<P_VER<<msgData.size()<<M_AskUList<<time;
    data.append(msgData);

    conn->write(data);
}

//protected:
//消息回调函数:
void OClientCore::msgLoginOk(QByteArray *data,unsigned int time)
{

}

void OClientCore::msgLoginError(QByteArray *data,unsigned int time)
{

}

void OClientCore::msgTime(QByteArray *data,unsigned int time)
{
    unsigned int ltime=QDateTime::currentDateTime().toTime_t();
    unsigned int sertime=QString(*data).toUInt();
    stime=sertime-ltime;
    uptime=1;
}

void OClientCore::msgChangeUList(QByteArray*,unsigned int time)
{
    msgAskUList();
}

void OClientCore::msgUList(QByteArray*,unsigned int time)
{

}

//protected:

//private slots:
void OClientCore::onData()
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

void OClientCore::onError(QAbstractSocket::SocketError s)
{
    //throwError();
}
