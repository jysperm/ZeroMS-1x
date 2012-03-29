#include "OClient.h"

OClient::OClient():isLoged(false),isShowIp(false),status(ONLINE)
{

}

OClient::~OClient()
{
    if(main && main->conn)
        main->conn->deleteLater();
    QMutableListIterator<OClientPeer*> i(subConnList);
    while(i.hasNext())
    {
        if(i.next()->conn)
            i.value()->conn->deleteLater();
    }
}

void OClient::init()
{
    connect(main->conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    main->client=this;
}

void OClient::addSubConn(OClientPeer *peer)
{
    peer->client=this;
    connect(peer,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    subConnList.push_back(peer);
}

void OClient::onError(OClientPeer *peer,QString msg,QAbstractSocket::SocketError s)
{
    if(peer==main)
    {
        emit lostMainConnect(this);
    }
    else
    {
        subConnList.removeOne(peer);
        delete peer;
    }
}
