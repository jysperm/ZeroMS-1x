#include "OProtocolForSC.h"
#include <QTcpSocket>
#include "../public/OMessage.h"
#include "../public/OGlobal.h"

OProtocolForSC::OProtocolForSC()
{
}

void OProtocolForSC::checkMsg(QString uname,QTcpSocket *conn,QByteArray *databuf)
{
    while(true)
    {
        OMessage msg=OMessage::fromDataBuff(databuf);
        if(msg.isEmpty())
            break;
        switch(msg.msgType)
        {
            case M_Login:
                ;
            default:
                ;
        }
    }
}
