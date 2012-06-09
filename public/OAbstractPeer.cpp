#include <QStringList>
#include "OAbstractPeer.h"
#include "global.h"
#include "../public/OSettings.h"

//public:
OAbstractPeer::OAbstractPeer(OPeerType peerType,QTcpSocket *connect):conn(connect),peerType(peerType)
{

}

OAbstractPeer::~OAbstractPeer()
{
    collect();
}

void OAbstractPeer::init()
{
    if(!conn)
        Q_ASSERT(!"需要在设置conn后调用该函数");

    connect(conn,SIGNAL(readyRead()),this,SLOT(checkMsg()));
    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
}

void OAbstractPeer::collect()
{
    if(conn)
    {
        conn->abort();
        conn->disconnect();
        conn->deleteLater();

        conn=0;
    }
}

void OAbstractPeer::Login(QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp)
{
    OMessage msg(M_Login);

    QStringList portList;
    QVectorIterator<int> i(p2pPort);
    while(i.hasNext())
    {
        portList.append(QString::number(i.next()));
    }

    msg.append(uname).aSpc().append(pwdHash).aSpc().append(portList.join(",")).aSpc();
    msg.append(isMain?MAIN:SUB).aSpc().append(isForce?FORCE:TRY).aSpc();
    msg.append(isShowIp?SHOWIP:HIDEIP);

    send(&msg);
}

void OAbstractPeer::LoginResult(QString status,QString ip)
{
    OMessage msg(M_LoginResult);
    msg.append(status).aSpc().append(ip);
    send(&msg);
}

void OAbstractPeer::Info(const QMap<QString, QString> &values)
{
    QByteArray data;
    QMapIterator<QString,QString> i(values);
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

void OAbstractPeer::AskPublicKey()
{
    OMessage msg(M_AskPublicKey);
    send(&msg);
}

void OAbstractPeer::PublicKey(QString publicKey)
{
    QByteArray key;
    key.append(publicKey);
    OMessage msg(M_PublicKey,key);
    send(&msg);
}

void OAbstractPeer::Unknown()
{
    QByteArray data;
    data.append((*config)["UNKNOWN"].toString());
    OMessage msg(M_Unknown,data);
    send(&msg);
    databuf.clear();
}

void OAbstractPeer::UserListChanged(QString listname)
{
    OMessage msg(M_UserListChanged);
    msg.append(listname);
    send(&msg);
}

void OAbstractPeer::AskUserList(QString listname,QString operation,bool isHasAvatar)
{
    OMessage msg(M_AskUserList);
    msg.append(operation).aSpc().append(isHasAvatar?AVATAR:NOAVATAR).aSpc().append(listname);
    send(&msg);
}

void OAbstractPeer::UserList(QString listname,QString operation,QVector<OUserlistItem> userlist)
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
            data=data.left(data.length()-1);

        if(i.hasNext())
            data.append(";");
    }
    OMessage msg(M_UserList,data);
    send(&msg);
}

void OAbstractPeer::SystemMsg(QString message)
{
    OMessage msg(M_SystemMsg);
    msg.append(message);
    send(&msg);
}

void OAbstractPeer::NewMsg(int id,QString view,QString uname,QString message)
{
    OMessage msg(M_NewMsg);
    msg.append(QString("%1 %2 %3 %4").arg(id).arg(view).arg(uname).arg(message));
    send(&msg);
}

void OAbstractPeer::NewRequest(int id,QString uname,QString invitation,QString message)
{
    QByteArray data;
    data.append(QString("%1 %2 %3 %4").arg(id).arg(uname).arg(invitation).arg(message));

    OMessage msg(M_NewRequest,data);
    send(&msg);
}

void OAbstractPeer::UserInfo(QString uname,QMap<QString,QString> values)
{
    QByteArray data;
    data.append(uname).append(" ");
    QMapIterator<QString,QString> i(values);
    while(i.hasNext())
    {
        i.next();
        data.append(QString("%1:%2").arg(i.key()).arg(QString(i.value()).replace(";","\\;")));
        if(i.hasNext())
            data.append(";");
    }
    OMessage msg(M_UserInfo,data);
    send(&msg);
}

void OAbstractPeer::ProcessError(QString errorName,QString other)
{
    OMessage msgMsg(M_ProcessError);
    msgMsg.append(QString::number(currentMsg->type)).aSpc().append(errorName).aSpc().append(other);
    send(&msgMsg);
}

void OAbstractPeer::NewGroupRequest(int id,QString uname,QString requester,QString message)
{
    //uname为小组名，含前缀星号
    QByteArray data;
    data.append(QString("%1 %2 %3 %4").arg(id).arg(uname).arg(requester).arg(message));

    OMessage msg(M_NewGroupRequest,data);
    send(&msg);
}

//public slots:
void OAbstractPeer::checkMsg()
{
    if(!conn->atEnd())
        databuf.append(conn->readAll());

    while(true)
    {
        OMessage msg=OMessage::fromDataBuff(&databuf);
        currentMsg=&msg;
        if(msg.isEmpty())
            break;

        switch(msg.type)
        {
            case M_Login:
                if(peerType==ClientPeer)
                {
                    QString uname=msg.split(0);
                    QString pwdHash=msg.split(1);
                    QStringList ports=msg.split(2).split(",");
                    QVector<int> p2pPort;

                    if(!ports.isEmpty() && !ports.first().isEmpty())
                    {
                        QListIterator<QString> i(ports);
                        while(i.hasNext())
                            p2pPort.append(i.next().toInt());
                    }

                    bool isMain=(msg.split(3)==SUB)?false:true;
                    bool isForce=(msg.split(4)==FORCE)?true:false;
                    bool isShowIp=(msg.split(5)==HIDEIP)?false:true;
                    emit onLogin(uname,pwdHash,p2pPort,isMain,isForce,isShowIp);
                }
                break;
            case M_LoginResult:
                if(peerType==ServerPeer)
                {
                    QString status=msg.split(0);
                    QString ip=msg.split(1);

                    emit onLoginResult(status,ip);
                }
                break;
            case M_AskInfo:
                {
                    QStringList keys=msg.split(0).split(",");
                    emit onAskInfo(keys);
                    break;
                }
            case M_AskPublicKey:
                if(peerType==ClientPeer)
                {
                    emit onAskPublicKey();
                }
                break;
            case M_PublicKey:
                if(peerType==ServerPeer)
                {
                    QString publickey=msg.splitTail(0);
                    emit onPublicKey(publickey);
                }
                break;
            case M_ModifyUserList:
                if(peerType==ClientPeer)
                {
                    QString listname=msg.split(0);
                    QString uname=msg.split(1);
                    QString operation=(msg.split(2)==REMOVE)?REMOVE:ADD;
                    QString messages=msg.split(3);
                    emit onModifyUserList(listname,uname,operation,messages);
                }
                break;
            case M_UserListChanged:
                if(peerType==ServerPeer)
                {
                    QString listname=msg.split(0);
                    emit onUserListChanged(listname);
                }
                break;
            case M_AskUserList:
                if(peerType==ClientPeer)
                {
                    QString operation=(msg.split(0)==ALL || msg.split(0)==DIFFONLY)?msg.split(0):ONLINE;
                    bool isHasAvatar=(msg.split(1)==AVATAR)?true:false;
                    QString listname=msg.split(2);
                    emit onAskUserList(listname,operation,isHasAvatar);
                }
                break;
            case M_UserList:
                if(peerType==ServerPeer)
                {
                    qDebug()<<msg.data;
                    QString operation=msg.split(0);
                    QString listname=msg.split(1);
                    QVector<OUserlistItem> userlist;

                    QStringList items=msg.split(2).split(";");
                    QListIterator<QString> i(items);
                    while(i.hasNext())
                    {
                        QStringList values=i.next().split(":");

                        OUserlistItem item;
                        item.uname=OGet(values,0);
                        item.status=OGet(values,1);
                        item.groupStatus=OGet(values,2);
                        item.ip=OGet(values,3);
                        QStringList ports=OGet(values,4).split(",");
                        QListIterator<QString> i(ports);
                        while(i.hasNext())
                            item.p2pPorts.append(i.next().toInt());
                        item.avatar=OGet(values,5);
                        userlist.append(item);
                    }

                    emit onUserList(listname,operation,userlist);
                }
                break;
            case M_State:
                if(peerType==ClientPeer)
                {
                    QString status=msg.split(0);
                    if(!(status==BORED || status==BUZY || status==AWAY))
                        status==ONLINE;
                    emit onState(status);
                }
                break;
            case M_Logout:
                if(peerType==ClientPeer)
                {
                    emit onLogout();
                }
                break;
            case M_SendMsg:
                if(peerType==ClientPeer)
                {
                    QString uname=msg.split(0);
                    QString message=msg.splitTail(1);
                    emit onSendMsg(uname,message);
                }
                break;
            case M_UserRequest:
                if(peerType==ClientPeer)
                {
                    QString uname=msg.split(0);
                    QString message=msg.splitTail(1);
                    emit onUserRequest(uname,message);
                }
                break;
            case M_RequestResult:
                if(peerType==ClientPeer)
                {
                    int id=msg.split(0).toInt();
                    QString result=msg.split(1);
                    if(result==ALLOW || result==DENY)
                        emit onRequestResult(id,result);
                    else
                        Unknown();
                }
                break;
            case M_ModifyGroup:
                if(peerType==ClientPeer)
                {
                    QString group=OGroupName(msg.split(0));
                    QString uname=msg.split(1);
                    QStringList operators=msg.split(2).split(",");
                    emit onModifyGroup(group,uname,operators);
                }
                break;
            case M_AskUserInfo:
                if(peerType==ClientPeer)
                {
                    QString uname=msg.split(0);
                    QStringList keys=msg.split(1).split(",");
                    emit onAskUserInfo(uname,keys);
                }
                break;
            case M_OK:
                {
                    QString id=msg.split(0);
                    emit onOK(id);
                    break;
                }
            case M_ModifyInfo:
                if(peerType==ClientPeer)
                {
                    QString uname=msg.split(0);
                    QMap<QString,QString> values;
                    QStringList strValues=msg.splitTail(1).split(";");
                    QListIterator<QString> i(strValues);
                    while(i.hasNext())
                    {
                        QStringList strPair=i.next().split(":");
                        if(!strPair.isEmpty() && !strPair.front().isEmpty())
                        {
                            values.insert(strPair.front(),i.peekPrevious().right(i.peekPrevious().length() - strPair.front().length() -1));
                        }
                    }
                    emit onModifyInfo(uname,values);
                }
                break;
            default:
                {
                    Unknown();
                    break;
                }
        }

    }
    currentMsg=0;
}

//protected slots:
void OAbstractPeer::onError(QAbstractSocket::SocketError s)
{
    if(conn)
    {
        //如果存在连接，那么发射error信号，然后断开conn的所有信号槽，回收conn，清空缓冲区
        //这个类的情况就是这样：如果出错了，就表示这个类将要被销毁了
        //发射信号也是为了通知它的所有者销毁该类
        emit error(this,conn->errorString(),s);
        collect();
    }
}
