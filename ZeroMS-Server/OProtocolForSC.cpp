#include <QTcpSocket>
#include <QDateTime>
#include <QStringList>
#include "OProtocolForSC.h"
#include "../public/OMessage.h"
#include "../public/OGlobal.h"

OProtocolForSC::OProtocolForSC()
{

}

void OProtocolForSC::PublicKey(OClient::Connect *connect,QString publicKey)
{
    QByteArray key;
    key.append(publicKey);
    OMessage msg(M_PublicKey,key);
    connect->send(&msg);
}

void UserListChanged(OClient::Connect *connect,QString listname)
{
    OMessage msg(M_UserListChanged);
    msg.append(listname);
    connect->send(&msg);
}

void OProtocolForSC::LoginResult(OClient::Connect *connect,QString status,QString ip)
{
    OMessage msg(M_LoginResult);
    msg.append(status).appendSpace().append(ip);
    connect->send(&msg);
}

void OProtocolForSC::Info(OClient::Connect *connect,QMap<QString,QString> keys)
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
    connect->send(&msg);
}

void OProtocolForSC::UserList(OClient::Connect *connect,QString listname,QString operation,QVector<OClient::UserlistItem> userlist)
{
    QByteArray data;
    data.append(QString("%1 %2 ").arg(operation).arg(listname));
    QVectorIterator<OClient::UserlistItem> i(userlist);
    while(i.hasNext())
    {
        OClient::UserlistItem item=i.next();
        QStringList p2pPorts;
        QVectorIterator<int> iP2p(item.p2pPorts);
        while(iP2p.hasNext())
        {
            p2pPorts.append(QString::number(iP2p.next()));
        }
        data.append(QString("%1:%2:%3:%4:%5:%6").arg(item.uname).arg(item.status).arg(item.groupStatus).
                    arg(item.ip).arg(p2pPorts.join(",")).arg(item.avatar));

        //去除末尾多余的冒号
        int iData=0;
        while(iData++)
        {
            if(data.right(iData).left(1)!=":")
                break;
        }
        data=data.left(data.length()-(iData-1));

        if(i.hasNext())
            data.append(";");
    }
    OMessage msg(M_UserList,data);
    connect->send(&msg);
}

void OProtocolForSC::Unknown(OClient::Connect *connect)
{
    QByteArray data;
    data.append((*config)["UNKNOWN"].toString());
    OMessage msgMsg(M_Unknown,data);
    connect->send(&msgMsg);
    connect->databuf.clear();
}

void OProtocolForSC::checkMsg(OClient::Connect *connect)
{
    while(true)
    {
        OMessage msg=OMessage::fromDataBuff(&connect->databuf);
        if(msg.isEmpty())
            break;
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
            emit Login(connect,uname,pwdHash,p2pPort,isMain,isForce,isShowIp);
            break;
        }
        case M_AskInfo:
        {
            QStringList keys=msg.split(0).split(",");
            emit AskInfo(connect,keys);
            break;
        }
        case M_AskPublicKey:
            emit AskPublicKey(connect);
            break;
        case M_ModifyUserList:
        {
            QString uname=msg.split(0);
            bool isAddOrRemove=(msg.split(1)==REMOVE)?false:true;
            emit ModifyUserList(connect,uname,isAddOrRemove);
            break;
        }
        case M_AskUserList:
        {
            QString operation=(msg.split(0)==ALL || msg.split(0)==DIFFONLY)?msg.split(0):ONLINE;
            bool isHasAvatar=(msg.split(1)==AVATAR)?true:false;
            QString listname=msg.split(2);
            emit AskUserList(connect,listname,operation,isHasAvatar);
            break;
        }
        default:
        {
            Unknown(connect);
        }
        }
    }
}
