#include "OProtocolForSC.h"
#include <QTcpSocket>
#include "../public/OMessage.h"
#include "../public/OGlobal.h"

OProtocolForSC::OProtocolForSC()
{

}

void OProtocolForSC::checkMsg(OClient::Connect *connect)
{
    while(true)
    {
        OMessage msg=OMessage::fromDataBuff(&connect->databuf);
        if(msg.isEmpty())
            break;
        switch(msg.msgType)
        {
            case M_AskPublicKey:
                ;
            default:
                ;
        }
    }
}
