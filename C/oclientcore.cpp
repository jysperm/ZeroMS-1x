#include <QAbstractSocket>
#include <QApplication>
#include <QByteArray>
#include <QDateTime>
#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include "const.h"
#include "inline.h"
#include "oclientcore.h"
#include "opacket.h"

//public:
OClientCore::OClientCore():conn(0),timeDiff(0),isLoged(0),databuf(0)
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
    connect(conn,SIGNAL(connected()),this,SIGNAL(onConnected()));
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
    OPacket packet(M_AskTime);
    conn->write(packet.exec());
}

void OClientCore::msgPing()
{
    OPacket packet(M_Ping);
    conn->write(packet.exec());
}

void OClientCore::msgExit()
{
    OPacket packet(M_Exit);
    conn->write(packet.exec());
}

void OClientCore::msgCMsg(QString objname,QString msg)
{
    QByteArray msgData;
    msgData.append(QString("%1 %2").arg(objname).arg(msg));
    OPacket packet(msgData,M_Error);
    conn->write(packet.exec());
}

void OClientCore::msgLogin(QString uname,QString pwd)
{
    //注意，调用该函数可能会引起阻塞，但在阻塞中会自动调用qApp->processEvents()
    unsigned int curTime=QDateTime::currentDateTime().toTime_t();
    unsigned int serTime=curTime+timeDiff;
    myname=uname;
    //下面这句就是等到离下次密码更新时间大于5秒的时候
    //....总之很绕口，大家看一下通讯协议
    while((serTime%(unsigned int)10)>5)
    {
        qApp->processEvents();
        curTime=QDateTime::currentDateTime().toTime_t();
        serTime=curTime+timeDiff;
    }

    unsigned int time=serTime-(serTime%10);
    QString spwd=md5(md5(QString::number(time))+md5(uname)+md5(pwd));

    QByteArray msgData;
    msgData.append(QString("%1 %2 %3 %4").arg(uname).arg(spwd).arg(CLIENT_VER_NUM).arg(CLIENT_NAME));
    OPacket packet(msgData,M_Login);
    conn->write(packet.exec());
}

void OClientCore::msgAskUList()
{
    OPacket packet(M_AskUList);
    conn->write(packet.exec());
}

//protected:
//可重载消息回调函数:
void OClientCore::msgSMsg(QByteArray *data,unsigned int time)
{

}

void OClientCore::msgLoginOk(QByteArray *data,unsigned int time)
{

}

void OClientCore::msgLoginError(QByteArray *data,unsigned int time)
{
    //如果不希望登陆错误后断开连接，请重载这个函数
    abort();
}

void OClientCore::msgUList(QByteArray *data,unsigned int time)
{

}

//private:
//不可重载消息回调函数:
void OClientCore::msgError(QByteArray *data,unsigned int time)
{
    //这里的表述不太严谨，应该是服务器不支持该版本的协议
    //但是考虑到一般用户的理解，先这么写
    Error(tr("服务器不支持该版本客户端，可能是客户端已经过期"));
}

void OClientCore::msgTime(QByteArray *data,unsigned int time)
{
    unsigned int curTime=QDateTime::currentDateTime().toTime_t();
    unsigned int serTime=QString(*data).toUInt();
    timeDiff=serTime-curTime;
    emit onTimeChange(timeDiff);
}

void OClientCore::msgChangeUList(QByteArray *data,unsigned int time)
{
    msgAskUList();
    emit onChangeUList();
}

void OClientCore::Error(QString msg)
{
    isLoged=0;
    abort();
    emit onError(msg);
}

//private slots:
void OClientCore::dataCome()
{
    emit onData();
    databuf->append(conn->readAll());
    if(databuf->size()<P_HEADLEN)
        return;
    int ver=QBtoint(databuf->mid(0,4));
    if(!checkVer(ver))
    {
        Error(tr("不支持的协议版本，可能是客户端已经过期"));
        return;
    }
    int len=QBtoint(databuf->mid(4,4));
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
                break;
            case M_SMsg:
                if(!isLoged)
                    break;
                msgSMsg(msgData,time);
                //可重载的消息回调函数都是在函数外来发射信号
                //否则的话，重载后将无法发射信号
            {
                QString msg=*msgData;
                //下面就是提取msgData中的各个字段
                QString objName=msg.left(msg.indexOf(" "));
                msg.remove(0,objName.length()+1);
                QString from=msg.left(msg.indexOf(" "));
                msg.remove(0,from.length()+1);
                QString uname=msg.left(msg.indexOf(" "));
                msg.remove(0,uname.length()+1);
                QString msgMsg=msg.left(msg.indexOf(" "));
                emit onSMsg(objName,from,uname,msgMsg);
            }
                break;
            case M_Time:
                msgTime(msgData,time);
                break;
            case M_LoginOk:
                if(isLoged)
                    break;
                msgLoginOk(msgData,time);
            {
                isLoged=1;
                emit onLoginOk();
            }
                break;
            case M_LoginError:
                if(isLoged)
                    break;
                msgLoginError(msgData,time);
                emit onLoginError();
                break;
            case M_UList:
                if(!isLoged)
                    break;
                msgUList(msgData,time);
            {
                QStringList users=QString(*msgData).split(",");
                emit onUList(users);
            }
                break;
            case M_ChangeUList:
                if(!isLoged)
                    break;
                msgChangeUList(msgData,time);
                break;
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
    Error(conn->errorString());
}
