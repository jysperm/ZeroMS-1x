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

void OProtocolForSC::UserInfo(OClient::Connect *connect,QString listname,QString operation,QVector<OClient::UserlistCache> userlist)
{

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
            if(ports.count()==1 && ports[0].isEmpty())
                ports=QStringList();
            QVector<int> p2pPort;
            for(QList<QString>::iterator i=ports.begin();i!=ports.end();++i)
                p2pPort.push_back((*i).toInt());
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
            QString operation=(msg.split(0)==ALL && msg.split(0)==DIFFONLY)?msg.split(0):ONLINE;
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
