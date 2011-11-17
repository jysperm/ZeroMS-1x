#include <QAbstractSocket>
#include <QByteArray>
#include <QDataStream>
#include <QDateTime>
#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTcpSocket>
#include "const.h"
#include "inline.h"
#include "oservercore.h"
#include "opacket.h"
#include "oclient.h"

extern QTextStream cout;

//public:
OServerCore::OServerCore():manager(0),reply(0)
{
    log(tr("0-ms start"));
}

OServerCore::~OServerCore()
{
    DELETE(reply);
    DELETE(manager);
    stop();
}

void OServerCore::run()
{
    listen(QHostAddress::Any,CLIENT_PORT);
    connect(this,SIGNAL(newConnection()),this,SLOT(onNewConn()));
    log(tr("listening,port %1").arg(QString::number(CLIENT_PORT)));
}

void OServerCore::stop()
{
    //终止监听并释放所有连接，请谨慎使用，这将导致所有用户掉线
    close();
    for(it i=cl.begin();i!=cl.end();i++)
    {
        delete i.value();
	cl.erase(i);
    }
    log(tr("0-ms stop listening"));
}

//private:
void OServerCore::log(QString msg)
{
    //负责打印日志，目前是输出到标准输出
    //建立这个函数的目的是为了方便今后修改输出的位置
    cout<<msg<<endl;
}

void OServerCore::checkMsg(QString uname)
{
    QByteArray *databuf=cl[uname]->databuf;
    int ver=QBtoint(databuf->mid(0,4));
    int len=QBtoint(databuf->mid(4,4));
    if(!checkVer(ver))
    {
        throwError(uname);
        return;
    }
    if(databuf->size()>=(len+P_HEADLEN))
    {
        //如果已经接收到了数据包的全部数据，进行分发命令
        int type=QBtoint(databuf->mid(8,4));
        unsigned int time=QBtoint(databuf->mid(12,4));
        QByteArray *msgData=new QByteArray(databuf->mid(P_HEADLEN,len));
        cl[uname]->lasttime=QDateTime::currentDateTime().toTime_t();
        switch(type)
        {
            case M_Error:
            case M_AskTime:
            case M_Time:
            case M_Ping:
            case M_Exit:
            case M_CMsg:
            case M_SMsg:
            case M_Login:
            case M_LoginOk:
            case M_LoginError:
            case M_AskUList:
            case M_UList:
            case M_ChangeUList:
                ;
        }
        DELETE(msgData);
        if(databuf)
            databuf->remove(0,P_HEADLEN+len);
    }
}

void OServerCore::throwError(QString uname)
{/*
    //向客户端抛出异样，并关闭连接
    //一般是无法解析客户端发来的消息时，使用此函数
    log(tr("%1 abort，cant understand").arg(uname));
    QByteArray data;
    QByteArray msgData;
    //请注意，只能用QDataStream处理数字类型，如果处理字符串的话，会被转换成QT专有的格式，而非UTF-8
    QDataStream DSdata(&data,QIODevice::ReadWrite);
    msgData.append(ERROR_STR);
    unsigned int time=QDateTime::currentDateTime().toTime_t();
    DSdata<<P_USE<<msgData.size()<<M1_Error<<time;
    data.append(msgData);
    QTcpSocket *conn=sl[uname]->conn;
    conn->write(data);
    sl[uname]->isClose=1;*/
}

//----
void OServerCore::msgError(QString uname)
{
    QByteArray msgData;
    msgData.append(ERROR_STR);
    OPacket packet(msgData,M_Error);
    QTcpSocket *conn=cl[uname]->conn;
    conn->write(packet.exec());
}

void OServerCore::msgAckTime(QString uname,QByteArray *data,unsigned int time)
{
    msgTime(uname);
}

void OServerCore::msgTime(QString uname)
{
    QByteArray msgData;
    //注意，这里的时间戳是字符串的形式
    msgData.append(QString::number(QDateTime::currentDateTime().toTime_t()));
    OPacket packet(msgData,M_Time);
    QTcpSocket *conn=cl[uname]->conn;
    conn->write(packet.exec());
}

void OServerCore::msgPing(QString uname,QByteArray *data,unsigned int time)
{

}

void OServerCore::msgExit(QString uname,QByteArray *data,unsigned int time)
{
    delete cl[uname];
    cl.remove(uname);
}

void OServerCore::msgCMsg(QString uname,QByteArray *data,unsigned int time)
{

}

void OServerCore::msgLogin(QString uname,QByteArray *data,unsigned int time)
{

    QString msg=*data;
    //下面几句比较乱，总之是为了从数据部分(msg)里拆分出各个字段
    QString msgUName=msg.left(msg.indexOf(" "));
    msg.remove(0,msgUName.length()+1);
    QString msgPwd=msg.left(msg.indexOf(" "));
    msg.remove(0,msgPwd.length()+1);
    QString msgClientVer=msg.left(msg.indexOf(" "));
    msg.remove(0,msgClientVer.length()+1);
    QString msgClientName=msg.left(msg.indexOf(" "));

    unsigned int stime=QDateTime::currentDateTime().toTime_t();
    QString dpwd=md5(API_KEY+msgPwd);
    QByteArray content;
    content.append(QString("do=login&uname=%1&listname=%2&pwd=%3&time=%4&clientver=%5&clientname=%6")
                   .arg(msgUName).arg(uname).arg(dpwd).arg(QString::number(stime)).arg(msgClientVer).arg(msgClientName));
    if(!manager)
        manager=new QNetworkAccessManager(this);
    QNetworkRequest request ;
    request.setUrl(QUrl(LOGIN_APIURL));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,content.length());
    reply=manager->post(request,content);
    connect(reply, SIGNAL(finished()), this, SLOT(LoginResult()));
}

void OServerCore::msgLoginOk(QString uname)
{
    OPacket packet(M_LoginOk);
    QTcpSocket *conn=cl[uname]->conn;
    conn->write(packet.exec());
}

void OServerCore::msgLoginError(QString uname)
{
    OPacket packet(M_LoginError);
    QTcpSocket *conn=cl[uname]->conn;
    conn->write(packet.exec());
}

void OServerCore::msgAskUList(QString uname,QByteArray *data,unsigned int time)
{
    if(cl[uname]->isLoged)
        msgUList(uname);
}

void OServerCore::msgUList(QString uname)
{
    QStringList users=cl.keys();
    QStringList online;
    foreach(QString i,users)
    {
        if(cl[i]->isLoged)
            online.append(i);
    }
    QByteArray msgData;
    msgData.append(users.join(","));
    OPacket packet(msgData,M_UList);
    QTcpSocket *conn=cl[uname]->conn;
    conn->write(packet.exec());
}

void OServerCore::msgChangeUList(QStringList users)
{
    foreach(QString i,users)
    {
        if(cl[i]->isLoged)
        {
            OPacket packet(M_ChangeUList);
            QTcpSocket *conn=cl[i]->conn;
            conn->write(packet.exec());
        }
    }
}

//private slots:
void OServerCore::LoginResult()
{
    QStringList result=QString::fromUtf8(reply->readAll()).split("\n");
    if(result[0].toInt())
    {
        if(cl.contains(result[UAPI_UNAME]))
        {
            delete cl[result[UAPI_UNAME]];
            cl.remove(result[UAPI_UNAME]);
        }
        OClient *pc=cl[result[UAPI_LISTNAME]];
        cl.remove(result[UAPI_LISTNAME]);
        cl.insert(result[UAPI_UNAME],pc);
        pc->isLoged=1;
        pc->clientver=result[UAPI_CLIENTVER].toInt();
        pc->clientname=result[UAPI_CLIENTNAME];
        msgChangeUList(cl.keys());
        msgLoginOk(result[UAPI_UNAME]);
    }
    else
    {
        msgLoginError(result[UAPI_LISTNAME]);
    }
    DELETE(reply);
}

void OServerCore::onNewConn()
{
    while(hasPendingConnections())
    {
	QTcpSocket *conn=nextPendingConnection();
	QString uname=QString("#%1:%2").arg(conn->peerAddress().toString()).arg(conn->peerPort());
	if(cl.size()<CLIENT_MAX)
	{
	    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
	    connect(conn,SIGNAL(readyRead()),this,SLOT(onData()));
            OClient *pc=new OClient;
	    pc->conn=conn;
            if(cl.contains(uname))
            {
                delete cl[uname];
                cl.remove(uname);
            }
	    cl.insert(uname,pc);
	    log(tr("%1 connected").arg(uname));
	}
	log(tr("over of connections up limit:%1").arg(CLIENT_MAX));
    }
}

void OServerCore::onData()
{
    //该函数把收到的数据存入连接对象的缓冲区，如果缓存区已经包含了一个数据包头，
    //则调用消息分发函数
    for(it i=cl.begin();i!=cl.end();i++)
    {
	QTcpSocket *conn=i.value()->conn;
	QByteArray *databuf=i.value()->databuf;
	if(!conn->atEnd())
	{
	    databuf->append(conn->readAll());
	    if(databuf->size()>=P_HEADLEN)
		checkMsg(i.key());
	}
    }
}

void OServerCore::onError(QAbstractSocket::SocketError s)
{
    for(it i=cl.begin();i!=cl.end();i++)
    {
	QTcpSocket *conn=i.value()->conn;
	if(conn->error()==s)
	{
	    //如果这个连接对象的上一个错误和接收到的错误号一样，
	    //说明是它出错，输出错误信息并断开连接
	    log(tr("%1 connect error：%2").arg(i.key()).arg(conn->errorString()));
	    int isLoged=i.value()->isLoged;
	    DELETE(i.value());
	    cl.erase(i);
	    if(isLoged)
                msgChangeUList(cl.keys());
	}
    }
}
