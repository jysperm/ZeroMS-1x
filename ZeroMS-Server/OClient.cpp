#include "OClient.h"

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
OClient::OClient():isLoged(false),isShowIp(false)
{

}

OClient::~OClient()
{
    if(main && main->conn)
        main->conn->deleteLater();
    for(SubConnIt i=subConnList.begin();i!=subConnList.end();++i)
        if((*i)->conn)
            (*i)->conn->deleteLater();
}

void OClient::init()
{
    if(main->conn)
    {
        connect(main->conn,SIGNAL(readyRead()),this,SLOT(onData()));
        connect(main->conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    }
}

void OClient::addSubConn(QTcpSocket *conn)
{
    connect(conn,SIGNAL(readyRead()),this,SLOT(onData()));
    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    subConnList.push_back(new Connect(conn,this));
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

//private slots:
void OClient::onData()
{
    checkData(main);
    for(SubConnIt i=subConnList.begin();i!=subConnList.end();++i)
    {
        checkData(*i);
    }
}

void OClient::onError(QAbstractSocket::SocketError s)
{
    //如果是主连接发生错误，便直接发射信号，等待服务器核心销毁自己
    //如果是次要连接发生错误，则先发射信号通知服务器核心，然后自己销毁这个连接

    if(main->conn->error()==s)
    {
        emit error(main,main->conn->errorString(),s);
    }
    else
    {
        for(SubConnIt i=subConnList.begin();i!=subConnList.end();++i)
        {
            QTcpSocket *conn=(*i)->conn;
            if(conn->error()==s)
            {
                emit error(*i,conn->errorString(),s);
                conn->deleteLater();
                subConnList.erase(i);
            }
        }
    }
}
