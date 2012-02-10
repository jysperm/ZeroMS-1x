#include "OMessage.h"

OMessage::OMessage():time(0),protocolVer(0),msgType(0)
{

}

OMessage::OMessage(QByteArray data)
{
    protocolVer=QBtoint(data.mid(0,4));
    int len=QBtoint(data.mid(4,4));
    if(len!=data.size()-P_HEADLEN)
        Q_ASSERT(!"传递进的数据长度必须和数据包头部等长");
    msgType=QBtoint(data.mid(8,4));
    time=QBtoint(data.mid(12,4));

    this->data=data.mid(P_HEADLEN);
}

OMessage OMessage::fromDataBuff(QByteArray *databuf,bool autoRemove)
{
    OMessage msg;
    if(databuf->size()>=P_HEADLEN)
    {
        int len=QBtoint(databuf->mid(4,4));
        if(databuf->size()>=len)
        {
            msg=OMessage(databuf->left(P_HEADLEN+len));
            if(autoRemove)
            {
                databuf->remove(0,P_HEADLEN+len);
            }
        }
    }
    return msg;
}
