#include <QAbstractSocket>
#include <QApplication>
#include <QByteArray>
#include <QDateTime>
#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include "const.h"
#include "../public/inline.h"
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
    connect(this,SIGNAL(onError(OClientCore::ErrorType,QString,QAbstractSocket::SocketError)),this,SLOT(Error(OClientCore::ErrorType,QString,QAbstractSocket::SocketError)));
}

void OClientCore::connectTo(QString ip,int port)
{
    abort();
    conn=new QTcpSocket;
    databuf=new QByteArray;
    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
    connect(conn,SIGNAL(readyRead()),this,SLOT(dataCome()));
    connect(conn,SIGNAL(connected()),this,SIGNAL(onConnected()));
    conn->connectToHost(QHostAddress(ip),port);
}

void OClientCore::abort()
{
    if(conn)
    {
        conn->abort();
        conn->waitForDisconnected();
    }
    DELETE(databuf);
    emit onAborted();
}

QString OClientCore::errorString(ErrorType e)
{
    int lastError=this->lastError;
    if(e!=-2)
        lastError=e;
    const QString Description[4]={tr("未知错误"),
                                  tr("未发现错误"),
                                  tr("无法理解服务器发来的命令，可能是协议过期"),
                                  tr("服务器无法理解客户端发送的命令，可能是协议过期")};
    if(lastError<3)
        return Description[lastError-1];
    else
        return conn->errorString();
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
    OPacket packet(msgData,M_CMsg);
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

void OClientCore::Error(OClientCore::ErrorType e,QString msg,QAbstractSocket::SocketError s)
{
    if(s>=3)
    {
        isLoged=0;
        abort();
    }
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
        emit onError(lastError=CantUnderstand,errorString((ErrorType)1),(QAbstractSocket::SocketError)0);
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
                emit onError(lastError=MsgError,*msgData,(QAbstractSocket::SocketError)0);
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
                QString msgMsg=msg;
                emit onSMsg(objName,from,msgMsg);
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
                emit onError(lastError=CantUnderstand,errorString((ErrorType)1),(QAbstractSocket::SocketError)0);
        }
        DELETE(msgData);
        if(databuf)
            databuf->remove(0,P_HEADLEN+len);
    }
}

void OClientCore::socketError(QAbstractSocket::SocketError s)
{
    if(s==0)
    {
        emit onError(lastError=SocketCantConnect,conn->errorString(),s);
        return;
    }
    if(s==1)
    {
        emit onError(lastError=SocketConnectionAbort,conn->errorString(),s);
        return;
    }
    emit onError(lastError=SocketOthers,conn->errorString(),s);
}
