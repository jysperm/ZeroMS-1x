#ifndef PUBLIC_OMESSAGE_H
#define PUBLIC_OMESSAGE_H

#include <QObject>
#include "global.h"

class OMessage
{
public:
    OMessage();
    OMessage(QByteArray data);

    static OMessage fromDataBuff(QByteArray *databuf,bool autoRemove=true);

    inline bool isEmpty();
    inline static int QBtoint(QByteArray b);

    QByteArray data;
    unsigned int time;
    int protocolVer;
    int msgType;
};

inline bool OMessage::isEmpty()
{
    if((!msgType) && !(data.isEmpty()))
        return true;
    else
        return false;
}

inline int OMessage::QBtoint(QByteArray b)
{
    QDataStream d(b);
    int i;
    d>>i;
    return i;
}

#endif // PUBLIC_OMESSAGE_H
