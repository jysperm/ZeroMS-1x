#include <QTcpSocket>
#include <QDateTime>
#include "OProtocolForSC.h"
#include "../public/OMessage.h"
#include "../public/OGlobal.h"

OProtocolForSC::OProtocolForSC()
{
    qsrand(QDateTime::currentDateTime().toTime_t());
}

void OProtocolForSC::PublicKey(OClient::Connect *connect)
{
    QByteArray key;

    //这里的公钥采用15个ascii从32到126的随机字符
    for(int i=0;i<15;i++)
    {
        char c = ( qrand()%(126-32) )+32;
        key.append(QString(c));
    }
    connect->publicKey=QString(key);

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
                    bool isMain=(msg.split(3)=="sub")?false:true;
                    bool isForce=(msg.split(4)=="force")?true:false;
                    bool isShowIp=(msg.split(5)=="hideip")?false:true;
                    emit Login(connect,uname,pwdHash,isMain,isForce,isShowIp);
                    break;
                }
            case M_AskPublicKey:
                emit AskPublicKey(connect);break;
            default:
                ;
        }
    }
}
