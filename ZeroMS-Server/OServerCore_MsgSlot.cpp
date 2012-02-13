#include "OServerCore.h"

//private slots:
void OServerCore::AskPublicKey(OClient::Connect *connect)
{
    protocol.PublicKey(connect);
    log(QString("%1 请求公钥").arg(connect->client->getSignature()));
}
