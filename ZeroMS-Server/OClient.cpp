#include "OClient.h"
#include "global.h"

OClient::OClient():isLoged(false),isShowIp(false),status(ONLINE)
{

}

OClient::~OClient()
{
    if(main)
        delete main;
    QMutableListIterator<OClientPeer*> i(subConnList);
    while(i.hasNext())
    {
        if(i.next()->conn)
            delete i.value();
    }
}

void OClient::init()
{
    connect(main,SIGNAL(error(OAbstractPeer*,QString,QAbstractSocket::SocketError)),this,SLOT(onError(OAbstractPeer*,QString,QAbstractSocket::SocketError)));
    main->client=this;
    main->init();
}

void OClient::addSubConn(OClientPeer *peer)
{
    peer->client=this;
    connect(peer,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    subConnList.push_back(peer);
    peer->init();
}

void OClient::onError(OAbstractPeer *peer,QString msg,QAbstractSocket::SocketError s)
{
    if(static_cast<OClientPeer*>(peer)==main)
    {
        emit lostMainConnect(this);
    }
    else
    {
        subConnList.removeOne(static_cast<OClientPeer*>(peer));
        delete peer;
    }
}
