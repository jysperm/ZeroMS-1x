#include <QCoreApplication>
#include <QDateTime>
#include "OServerCore.h"

//public:
OServerCore::OServerCore():cin(stdin),cout(stdout)
{
    qsrand(QDateTime::currentDateTime().toTime_t());
}

void OServerCore::init()
{
    //使用该类时应该在创建类后立刻调用该函数进行初始化，因为在构造函数中无法调用虚函数
    log(tr("零毫秒服务器启动 %1 %2").arg(SERVER).arg(::VERSION));
    connect(&server,SIGNAL(newConnection()),this,SLOT(onNewConn()));

    //绑定OProtocolForSC相关的信号槽
    connect(&protocol,SIGNAL(AskPublicKey(OClient::Connect*)),this,SLOT(AskPublicKey(OClient::Connect*)));
    connect(&protocol,SIGNAL(Login(OClient::Connect*,QString,QString,QVector<int>,bool,bool,bool)),this,SLOT(Login(OClient::Connect*,QString,QString,QVector<int>,bool,bool,bool)));
    connect(&protocol,SIGNAL(AskInfo(OClient::Connect*,QStringList)),this,SLOT(AskInfo(OClient::Connect*,QStringList)));
    connect(&protocol,SIGNAL(ModifyUserList(OClient::Connect*,QString,bool)),this,SLOT(ModifyUserList(OClient::Connect*,QString,bool)));
    connect(&protocol,SIGNAL(AskUserList(OClient::Connect*,QString,QString,bool)),this,SLOT(AskUserList(OClient::Connect*,QString,QString,bool)));
    connect(&protocol,SIGNAL(State(OClient::Connect*,QString)),this,SLOT(State(OClient::Connect*,QString)));

    //注册该类型，以便可以在信号槽中作为参数传递
    //与此对应的还有OClient.h中结尾的Q_DECLARE_METATYPE(OClient::Connect)
    qRegisterMetaType<OClient::Connect>("OClient::Connect");

    //用于AskInfo
    info.insert(OFFLINECACHETIME,(*config)["OFFLINE_CACHE_TIME"].toString());
    info.insert(NOACTIVITYTIME,(*config)["NOACTIVITY_TIME"].toString());
    info.insert(Protocol2::VERSION,QString("%1 %2").arg(SERVER).arg(::VERSION));
    info.insert(Protocol2::PROTOCOL,::PROTOCOL);
    info.insert(Protocol2::VERNUM,QString::number(::VERNUM));
    info.insert(WEBSITE,(*config)["WEBSITE"].toString());
}

void OServerCore::start()
{
    int port=(*config)["SERVER_PORT"].toInt();
    int maxClient=(*config)["CLIENT_MAX"].toInt();
    server.listen(QHostAddress::Any,port);
    server.setMaxPendingConnections(maxClient);
    log(tr("开始监听%1端口,最大连接数为%2").arg(port).arg(maxClient));
}

//private:
void OServerCore::log(QString msg)
{
    cout<<msg<<endl;
}

QString OServerCore::getUserStatus(QString uname)
{
    if(db.checkUser(uname))
    {//如果存在这个用户
        if(cl.contains(uname))
        {//如果这个用户在线
            return cl[uname]->status;
        }
        else
        {//如果这个用户不在线
            return OFFLINE;
        }
    }
    else
    {//如果不存在这个用户
        return REMOVED;
    }
}

void OServerCore::userListChange(QString uname)
{
    QVector<ODataBase::UserListItem> userlist=db.getUserListByUser(uname);
    QVectorIterator<ODataBase::UserListItem> iUserList(userlist);
    while(iUserList.hasNext())
    {
        ODataBase::UserListItem item=iUserList.next();

        if(cl.contains(item.uname))
        {
            protocol.UserListChanged(cl[item.uname]->main,item.uname);
        }
    }

    QVector<QString> groups=db.getAllGroup(uname);
    QVectorIterator<QString> iGroups(groups);
    while(iGroups.hasNext())
    {
        QString group=iGroups.next();
        QVector<QString> members=db.getGroupMembers(group);

        QVectorIterator<QString> iMember(members);
        while(iMember.hasNext())
        {
            QString user=iMember.next();

            if(cl.contains(user))
            {
                protocol.UserListChanged(cl[user]->main,QString("*%1").arg(group));
            }
        }
    }

}

//private slots:
void OServerCore::onNewConn()
{
    while(server.hasPendingConnections())
    {
        QTcpSocket *conn=server.nextPendingConnection();
        int maxClient=(*config)["CLIENT_MAX"].toInt();
        if(cl.size() < maxClient)
        {
            OClient *client=new OClient;
            OClient::Connect *mainConn=new OClient::Connect(conn,client);
            client->main=mainConn;
            connect(client,SIGNAL(error(OClient::Connect*,QString,QAbstractSocket::SocketError)),this,SLOT(onError(OClient::Connect*,QString,QAbstractSocket::SocketError)));
            connect(client,SIGNAL(newMsgData(OClient::Connect*)),&protocol,SLOT(checkMsg(OClient::Connect*)));
            client->init();
            QString uname=client->getSignature();
            if(cl.contains(uname))
            {
                //这里的情况是，存在与新连接同IP同端口的连接
                //这里显然应该直接销毁旧的连接
                delete cl[uname];
                cl.remove(uname);
            }
            cl.insert(uname,client);
            log(tr("%1 连接到服务器").arg(uname));
        }
        else
        {
            conn->abort();
            log(tr("超过了服务器最大客户端上限:%1").arg(maxClient));
        }
    }
}

void OServerCore::onError(OClient::Connect *connect,QString msg,QAbstractSocket::SocketError s)
{
    log(tr("%1(%2) 因错误断开连接:%3").arg(connect->client->getSignature()).arg((connect->isMain())?"主要":"次要").arg(msg));
    if(connect->isMain())
    {
        QString uname=connect->client->getSignature();
        delete cl[uname];
        cl.remove(uname);
        userListChange(uname);
    }
}

