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
                    for(QList<QString>::iterator i=ports.begin();i!=ports.end();++i)
                        p2pPort.push_back((*i).toInt());
                    bool isMain=(msg.split(3)=="sub")?false:true;
                    bool isForce=(msg.split(4)=="force")?true:false;
                    bool isShowIp=(msg.split(5)=="hideip")?false:true;
                    emit Login(connect,uname,pwdHash,p2pPort,isMain,isForce,isShowIp);
                    break;
                }
            case M_AskPublicKey:
                emit AskPublicKey(connect);break;
            default:
                ;
        }
    }
}
