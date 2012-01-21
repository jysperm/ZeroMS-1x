#include "OServerCore.h"

OServerCore::OServerCore():cin(stdin),cout(stdout)
{

}

void OServerCore::init()
{
    //使用该类时应该在创建类后立刻调用该函数进行初始化，因为在构造函数中无法调用虚函数

    log(tr("零毫秒服务器启动 %1 %2").arg(SERVER).arg(VERSION));
}

void OServerCore::start()
{
    int port=config["SERVER_PORT"].toInt();
    int maxClient=config["CLIENT_MAX"].toInt();
    server.listen(QHostAddress::Any,port);
    server.setMaxPendingConnections(maxClient);
    log(tr("开始监听%1端口,最大连接数为%2").arg(port).arg(maxClient));
}

void OServerCore::log(QString msg)
{
    cout<<msg<<endl;
}

void OServerCore::onNewConn()
{
    while(server.hasPendingConnections())
    {
        QTcpSocket *conn=server.nextPendingConnection();
        QString uname=QString("#%1:%2").arg(conn->peerAddress().toString()).arg(conn->peerPort());
        if(cl.size()<config["CLIENT_MAX"].toInt())
        {
            connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
            connect(conn,SIGNAL(readyRead()),this,SLOT(onData()));
            OClient *pc=new OClient;
            pc->conn=conn;
            if(cl.contains(uname))
            {
                delete cl[uname];
                cl.remove(uname);
            }
            cl.insert(uname,pc);
            log(tr("%1 connected").arg(uname));
        }
        else
        {
            conn->abort();
            log(tr("over of connections up limit:%1").arg(config["CLIENT_MAX"].toInt()));
        }
    }
}
