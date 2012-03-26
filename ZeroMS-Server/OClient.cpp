#include "OClient.h"

OPeerType OClientPeer::getPeerType()
{
    return ClientPeer;
}

OClientPeer(QTcpSocket *connect):OAbstractPeer(connect)
{

}

//class OClient::Connect
//public:
OClient::Connect::Connect():conn(0),client(0)
{

}

OClient::Connect::Connect(OClient *client):client(client)
{

}

OClient::Connect::Connect(QTcpSocket *conn,OClient *client):conn(conn),client(client)
{

}

//class OClient
//public:
OClient::OClient():isLoged(false),isShowIp(false),status(ONLINE)
{

}

OClient::~OClient()
{
    if(main && main->conn)
        main->conn->deleteLater();
    QMutableVectorIterator<Connect*> i(subConnList);
    while(i.hasNext())
    {
        if(i.next()->conn)
            i.value()->conn->deleteLater();
    }
}

void OClient::init()
{
    connect(main->conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
}

void OClient::addSubConn(OClientPeer *peer)
{
    peer->client=this;
    connect(peer,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    subConnList.push_back(peer);
}

void OClient::checkData(Connect *connect)
{
    if(!connect->conn->atEnd())
    {
        connect->databuf.append(connect->conn->readAll());
        if(connect->databuf.size() >= P_HEADLEN)
            emit newMsgData(connect);
    }
}

void OClient::onError(OClientPeer *peer,QString msg,QAbstractSocket::SocketError s)
{
    if(peer==&main)
    {
        emit lostMainConnect(this);
    }
    else
    {
        subConnList.remove(peer);
        delete peer;
    }
}
