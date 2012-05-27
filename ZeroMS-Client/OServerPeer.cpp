#include "OServerPeer.h"

OServerPeer::OServerPeer(QTcpSocket *connect):OAbstractPeer(ClientPeer,connect)
{

}

void OServerPeer::init()
{

    OAbstractPeer::init();
}
