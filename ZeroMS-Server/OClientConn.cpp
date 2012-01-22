#include "OClientConn.h"
#include "OServerCore.h"

extern OServerCore *servercore;

OClientConn::OClientConn():isLoged(false)
{

}

OClientConn::~OClientConn()
{
    if(conn)
        conn->deleteLater();
    for(SubConnIt i;i!=subConnList.end();++i)
        if((*i)->conn)
            (*i)->conn->deleteLater();
}

void OClientConn::init()
{
    if(conn)
    {
        connect(conn,SIGNAL(readyRead()),this,SLOT(onData()));
        connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    }
}

void OClientConn::addSubConn(QTcpSocket *conn)
{
    connect(conn,SIGNAL(readyRead()),this,SLOT(onData()));
    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    subConnList.push_back(new SubConn(conn));
}

void OClientConn::checkData(QTcpSocket *conn,QByteArray *databuf)
{
    if(conn->atEnd())
    {
        databuf->append(conn->readAll());
        if(databuf->size() >= P_HEADLEN)
            servercore->checkMsg(uname,conn,databuf);
    }
}

void OClientConn::onData()
{
    checkData(conn,&databuf);
    for(SubConnIt i;i!=subConnList.end();++i)
    {
        checkData((*i)->conn,&((*i)->databuf));
    }
}

void OClientConn::onError(QAbstractSocket::SocketError)
{

}
