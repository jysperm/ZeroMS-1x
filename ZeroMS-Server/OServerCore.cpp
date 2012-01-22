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

void OServerCore::checkMsg(QString uname,QTcpSocket *conn,QByteArray *databuf)
{

}

void OServerCore::onNewConn()
{
    while(server.hasPendingConnections())
    {
        QTcpSocket *conn=server.nextPendingConnection();
        int maxClient=config["CLIENT_MAX"].toInt();
        if(cl.size() < maxClient)
        {
            OClientConn *cConn=new OClientConn;
            cConn->conn=conn;
            QString uname=cConn->getSignature();
            if(cl.contains(uname))
            {
                //这里的情况是，存在与新连接同IP同端口的连接
                //这里显然应该直接销毁旧的连接
                delete cl[uname];
                cl.remove(uname);
            }
            cl.insert(uname,cConn);
            log(tr("%1 连接到服务器").arg(uname));
        }
        else
        {
            conn->abort();
            log(tr("超过了服务器最大客户端上限:%1").arg(maxClient));
        }
    }
}
