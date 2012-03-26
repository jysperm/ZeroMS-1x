#include <QTcpSocket>
#include <QDateTime>
#include <QStringList>
#include "OAbstractPeer.h"
#include "OMessage.h"

OAbstractPeer::OAbstractPeer(QTcpSocket *connect):conn(connect)
{

}

OAbstractPeer::~OAbstractPeer()
{
    collect();
}

void OAbstractPeer::onError(QAbstractSocket::SocketError s)
{
    if(conn)
    {
        //如果存在连接，那么发射error信号，然后断开conn的所有信号槽，回收conn，清空缓冲区
        //这个类的情况就是这样：如果出错了，就表示这个类将要被销毁了
        //发射信号也是为了通知它的所有者销毁该类
        emit error(this,conn->errorString(),s);
        disconnect(conn,this);
        collect();
    }
}

void OAbstractPeer::init()
{
    if(!conn)
        qErrnoWarning(Q_FUNC_INFO+tr(" 必须在给conn设置值后再调用init()"));
    connect(conn,SIGNAL(readyRead()),this,SLOT(checkMsg()));
    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
}

void OAbstractPeer::collect()
{
    delete conn;
    conn=0;
}

void OAbstractPeer::PublicKey(QString publicKey)
{
    QByteArray key;
    key.append(publicKey);
    OMessage msg(M_PublicKey,key);
    send(&msg);
}

void OAbstractPeer::UserListChanged(QString listname)
{
    OMessage msg(M_UserListChanged);
    msg.append(listname);
    send(&msg);
}

void OAbstractPeer::LoginResult(QString status,QString ip)
{
    OMessage msg(M_LoginResult);
    msg.append(status).aSpc().append(ip);
    send(&msg);
}

void OAbstractPeer::Info(QMap<QString,QString> keys)
{
    QByteArray data;
    QMapIterator<QString,QString> i(keys);
    while(i.hasNext())
    {
        i.next();
        data.append(QString("%1:%2").arg(i.key()).arg(QString(i.value()).replace(";","\\;")));
        if(i.hasNext())
            data.append(";");
    }
    OMessage msg(M_Info,data);
    send(&msg);
}

void OAbstractPeer::UserList(QString listname,QString operation,QVector<OClient::UserlistItem> userlist)
{
    QByteArray data;
    data.append(QString("%1 %2 ").arg(operation).arg(listname));
    QVectorIterator<OUserlistItem> i(userlist);
    while(i.hasNext())
    {
        OUserlistItem item=i.next();
        QStringList p2pPorts;
        QVectorIterator<int> iP2p(item.p2pPorts);
        while(iP2p.hasNext())
        {
            p2pPorts.append(QString::number(iP2p.next()));
        }
        data.append(QString("%1:%2:%3:%4:%5:%6").arg(item.uname).arg(item.status).arg(item.groupStatus).
                    arg(item.ip).arg(p2pPorts.join(",")).arg(item.avatar));

        //去除末尾多余的冒号
        while(QString(data.right(1))==":")
            data.remove(data.length()-1-1,1);

        if(i.hasNext())
            data.append(";");
    }
    OMessage msg(M_UserList,data);
    send(&msg);
}

void OAbstractPeer::Unknown()
{
    QByteArray data;
    data.append((*config)["UNKNOWN"].toString());
    OMessage msgMsg(M_Unknown,data);
    send(&msgMsg);
    databuf.clear();
}

void OAbstractPeer::checkMsg()
{
    if(!conn->atEnd())
        databuf.append(conn->readAll());

    while(true)
    {
        OMessage msg=OMessage::fromDataBuff(&databuf);
        if(msg.isEmpty())
            break;
        if(getPeerType()==ClientPeer)
        {
            switch(msg.type)
            {
                case M_Login:
                {
                    QString uname=msg.split(0);
                    QString pwdHash=msg.split(1);
                    QStringList ports=msg.split(2).split(",");
                    QVector<int> p2pPort;

                    QListIterator<QString> i(ports);
                    while(i.hasNext())
                        p2pPort.append(i.next().toInt());
                    bool isMain=(msg.split(3)==SUB)?false:true;
                    bool isForce=(msg.split(4)==FORCE)?true:false;
                    bool isShowIp=(msg.split(5)==HIDEIP)?false:true;
                    emit Login(uname,pwdHash,p2pPort,isMain,isForce,isShowIp);
                    break;
                }
                case M_AskInfo:
                {
                    QStringList keys=msg.split(0).split(",");
                    emit AskInfo(connect,keys);
                    break;
                }
                case M_AskPublicKey:
                    emit AskPublicKey();
                    break;
                case M_ModifyUserList:
                {
                    QString listname=msg.split(0);
                    QString uname=msg.split(1);
                    bool isAddOrRemove=(msg.split(2)==REMOVE)?false:true;
                    QString messages=msg.split(3);
                    emit ModifyUserList(listname,uname,isAddOrRemove,messages);
                    break;
                }
                case M_AskUserList:
                {
                    QString operation=(msg.split(0)==ALL || msg.split(0)==DIFFONLY)?msg.split(0):ONLINE;
                    bool isHasAvatar=(msg.split(1)==AVATAR)?true:false;
                    QString listname=msg.split(2);
                    emit AskUserList(listname,operation,isHasAvatar);
                    break;
                }
                case M_State:
                {
                    QString status=msg.split(0);
                    if(!(status==BORED || status==BUZY || status==AWAY))
                        status==ONLINE;
                    emit State(status);
                    break;
                }
                default:
                {
                    Unknown();
                }
            }
        }
        else
        {
            switch(msg.type)
            {
                default:
                {
                    Unknown();
                }
            }
        }
    }
}

