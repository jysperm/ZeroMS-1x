#include "OClientConn.h"

OClientConn::OClientConn():isLoged(false)
{

}

OClientConn::~OClientConn()
{
    if(conn)
        conn->deleteLater();
    for(SubConnIt i=subConnList.begin();i!=subConnList.end();++i)
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
            emit newMsgData(uname,conn,databuf);
    }
}

void OClientConn::onData()
{
    checkData(conn,&databuf);
    for(SubConnIt i=subConnList.begin();i!=subConnList.end();++i)
    {
        checkData((*i)->conn,&((*i)->databuf));
    }
}

void OClientConn::onError(QAbstractSocket::SocketError s)
{
    if(conn->error()==s)
    {
        emit error(getSignature(),conn->errorString(),s,true);
    }
    else
    {
        for(SubConnIt i=subConnList.begin();i!=subConnList.end();++i)
        {
            QTcpSocket *conn=(*i)->conn;
            if(conn->error()==s)
            {
                conn->deleteLater();
                subConnList.erase(i);
                emit error(getSignature(),conn->errorString(),s,false);
            }
        }
    }
}
