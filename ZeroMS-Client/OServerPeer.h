#ifndef OSERVERPEER_H
#define OSERVERPEER_H

#include "../public/OAbstractPeer.h"

class OServerPeer:public OAbstractPeer
{
    Q_OBJECT
public:
    explicit OServerPeer(QTcpSocket *connect=0);
};

#endif // OSERVERPEER_H
