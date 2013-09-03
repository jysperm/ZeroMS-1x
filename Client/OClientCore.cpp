#include "OClientCore.h"

//public:
OClientCore::OClientCore():conn(0),timeDiff(0),lastMsgTime(0),isLoged(0),timeOffLine(200),databuf(0),pingTimer(0)
{

}

OClientCore::~OClientCore()
{
    abort();
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
    connect(conn,SIGNAL(readyRead()),this,SIGNAL(onData()));
    connect(conn,SIGNAL(connected()),this,SIGNAL(onConnected()));
    conn->connectToHost(QHostAddress(ip),port);
}

void OClientCore::abort()
{
    delete pingTimer;
    pingTimer = 0;
    if(conn)
    {
        conn->abort();
        conn->waitForDisconnected();
    }
    delete conn;
    conn = 0;
    delete databuf;
    databuf = 0;
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
        return conn?conn->errorString():QString();
}

//public slots:
//消息发送函数:
void OClientCore::msgAskTime()
{
    OPacket packet(M_AskTime);
    sendPacket(packet);
}

void OClientCore::msgPing()
{
    pingUpdate();
    OPacket packet(M_Ping);
    sendPacket(packet);
}

void OClientCore::msgExit()
{
    OPacket packet(M_Exit);
    sendPacket(packet);
}

void OClientCore::msgCMsg(QString objname,QString msg)
{
    pingUpdate();
    QByteArray msgData;
    msgData.append(QString("%1 %2").arg(objname).arg(msg));
    OPacket packet(msgData,M_CMsg);
    sendPacket(packet);
}

void OClientCore::msgLogin(QString uname, QString pwd)
{
    pingUpdate();
    myname=uname;

    QByteArray msgData;
    msgData.append(QString("%1 %2 %3 %4").arg(uname).arg(pwd).arg(CLIENT_VER_NUM).arg(CLIENT_NAME));
    OPacket packet(msgData,M_LoginNoTimeVerify);
    sendPacket(packet);
}

void OClientCore::msgAskUList()
{
    pingUpdate();
    OPacket packet(M_AskUList);
    sendPacket(packet);
}

//protected:
//可重载消息回调函数:
void OClientCore::msgSMsg(OPacket &packet)
{

}

void OClientCore::msgLoginOk(OPacket &packet)
{

}

void OClientCore::msgLoginError(OPacket &packet)
{
    //如果不希望登陆错误后断开连接，请重载这个函数
    abort();
}

void OClientCore::msgUList(OPacket &packet)
{

}

void OClientCore::sendPacket(OPacket &packet)
{
    if(conn)
        conn->write(packet.exec());
}

void OClientCore::receivePacket(OPacket &packet)
{
    switch(packet.type)
    {
        case M_Error:
            emit onError(lastError=MsgError,packet.data,(QAbstractSocket::SocketError)0);
            return;
        case M_SMsg:
            if(!isLoged)
            {
                qDebug()<<tr("未登陆，但收到服务器的M_SMsg消息");
                break;
            }
            msgSMsg(packet);
            //可重载的消息回调函数都是在函数外来发射信号
            //否则的话，重载后将无法发射信号
            {
                QString user=packet.split(0);
                QString view=packet.split(1);
                QString msg=packet.splitTail(2);
                emit onSMsg(user,view,msg);
            }
            return;
        case M_Time:
            msgTime(packet);
            return;
        case M_LoginOk:
            if(isLoged)
            {
                qDebug()<<tr("已经成功登陆，但收到服务器的M_LoginOk消息");
                break;
            }
            msgLoginOk(packet);
            {
                pingTimer=new QTimer;
                connect(pingTimer,SIGNAL(timeout()),this,SLOT(pingTimeOut()));
                pingTimer->start(timeOffLine*1000*0.4);
                isLoged=1;
            }
            emit onLoginOk();
            return;
        case M_LoginError:
            if(isLoged)
            {
                qDebug()<<tr("已经成功登陆，但收到服务器的M_LoginError消息");
                break;
            }
            msgLoginError(packet);
            emit onLoginError();
            return;
        case M_UList:
            if(!isLoged)
            {
                qDebug()<<tr("未登陆，但收到服务器的M_UList消息");
                break;
            }
            msgUList(packet);
            {
                QStringList users=QString(packet.data).split(",");
                emit onUList(users);
            }
            return;
        case M_ChangeUList:
            if(!isLoged)
            {
                qDebug()<<tr("未登陆，但收到服务器的M_ChangeUList消息");
                break;
            }
            msgChangeUList(packet);
            return;
        default:
            break;
    }
    emit onError(lastError=CantUnderstand,errorString((ErrorType)1),(QAbstractSocket::SocketError)0);
}

//protected slots:
void OClientCore::Error(OClientCore::ErrorType e,QString msg,QAbstractSocket::SocketError s)
{
    if(s>=3)
    {
        isLoged=0;
        abort();
    }
}

//private:
//不可重载消息回调函数:
void OClientCore::msgTime(OPacket &packet)
{
    unsigned int curTime=QDateTime::currentDateTime().toTime_t();
    unsigned int serTime=QString(packet.data).toUInt();
    timeDiff=serTime-curTime;
    emit onTimeChange(timeDiff);
}

void OClientCore::msgChangeUList(OPacket &packet)
{
    msgAskUList();
    emit onChangeUList();
}

//private slots:
void OClientCore::pingTimeOut()
{
    //0.002f是0.001*2，前者是毫秒和秒之间的换算，后者为了在timeOffLine的一半时间内发送msgPing()消息
    //TODO，提前一半时间就发送相应消息?这里有待商量
    if((QDateTime::currentDateTime().toTime_t()-lastMsgTime)>(timeOffLine-pingTimer->interval()*0.002f))
        msgPing();
}

void OClientCore::dataCome()
{
    //该函数将收到的数据拆分为消息(一个OPacket对象)，交给receivePacket()函数
    databuf->append(conn->readAll());
    while(databuf && databuf->size() >= P_HEADLEN)
    {
        int ver=QBtoint(databuf->mid(0,4));
        if(!checkVer(ver))
        {
            emit onError(lastError=CantUnderstand,errorString((ErrorType)1),(QAbstractSocket::SocketError)0);
            databuf->clear();
            continue;
        }

        int len=QBtoint(databuf->mid(4,4));
        int type=QBtoint(databuf->mid(8,4));
        unsigned int time=QBtoint(databuf->mid(12,4));

        OPacket packet(databuf->mid(P_HEADLEN,len),type);
        packet.time=time;

        receivePacket(packet);

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
