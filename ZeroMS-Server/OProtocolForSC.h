#ifndef OPROTOCOLFORSC_H
#define OPROTOCOLFORSC_H

#include <QObject>
#include "OClient.h"
class QTcpSocket;

class OProtocolForSC:public QObject
{
    Q_OBJECT
public:
    explicit OProtocolForSC();
public slots:
    void checkMsg(OClient::Connect *connect);
signals:
    void AskPublicKey(OClient::Connect *connect);
};

#endif // OPROTOCOLFORSC_H
