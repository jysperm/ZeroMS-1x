#include <QDateTime>
#include "OServerCore.h"
#include "../public/OSettings.h"
#include "OClient.h"

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

    //用于AskInfo
    info.insert(OFFLINECACHETIME,(*config)["OFFLINE_CACHE_TIME"].toString());
    info.insert(NOACTIVITYTIME,(*config)["NOACTIVITY_TIME"].toString());
    info.insert(Protocol2::VERSION,QString("%1 %2").arg(SERVER).arg(::VERSION).replace(";","\\;"));
    info.insert(Protocol2::PROTOCOL,QString(::PROTOCOL).replace(";","\\;"));
    info.insert(Protocol2::VERNUM,QString::number(::VERNUM));
    info.insert(WEBSITE,(*config)["WEBSITE"].toString().replace(";","\\;"));
    info.insert(BUILDTIME,QString("%1 %2").arg(__DATE__).arg(__TIME__).replace(";","\\;"));
}

void OServerCore::start()
{
    int port=(*config)["SERVER_PORT"].toInt();
    int maxClient=(*config)["CLIENT_MAX"].toInt();
    server.listen(QHostAddress::Any,port);
    server.setMaxPendingConnections(maxClient);
    log(tr("开始监听%1端口,最大连接数为%2").arg(port).arg(maxClient));

    upTime=QDateTime::currentDateTime().toTime_t();
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
    using namespace OSDB;

    if(uname.left(1)=="*")
    {//如果uname是一个用户

        QVector<UserList> userlist=db.select<UserList>(OT(UserList::_user,uname));
        QVectorIterator<UserList> iUserList(userlist);
        while(iUserList.hasNext())
        {
            UserList item=iUserList.next();

            if(cl.contains(item.uname))
            {
                cl[item.uname]->main->UserListChanged(item.uname);
            }
        }

        QVector<GroupMember> groups=db.select<GroupMember>(OT(GroupMember::_uname,uname));
        QVectorIterator<GroupMember> iGroups(groups);
        while(iGroups.hasNext())
        {
            QString group=iGroups.next().groupname;

            QVector<GroupMember> members=db.select<GroupMember>(OT(GroupMember::_groupname,group));

            QVectorIterator<GroupMember> iMember(members);
            while(iMember.hasNext())
            {
                QString user=iMember.next().uname;

                if(cl.contains(user))
                {
                    cl[user]->main->UserListChanged(QString("*%1").arg(group));
                }
            }
        }
    }
    else
    {//如果uname是一个小组
        QString group=uname.right(uname.length()-1);

        QVector<GroupMember> groupsmembers=db.select<GroupMember>(OT(GroupMember::_groupname,group));
        QVectorIterator<GroupMember> i(groupsmembers);
        while(i.hasNext())
        {
            QString user=i.next().uname;

            if(cl.contains(user))
            {
                cl[user]->main->UserListChanged(group);
            }
        }
    }
}

void OServerCore::processRequest(int id)
{
    using namespace OSDB;

    if(id>-1)
    {//如果指定了请求id
        UserRequest request=db.selectFrist<UserRequest>( OT(UserRequest::_id,id) );
        if(!request._isEmpty)
        {//如果查询结果不是空的
            if(!request.isHandle)
            {//如果请求还没有被处理
                if(!OIsGroup(request.user))
                {//如果请求的目标是一个用户
                    if(OIsOnline(request.user))
                    {//如果请求的目标用户在线
                        cl[request.user]->main->NewRequest(id,request.uname,request.invitation,request.msg);
                    }
                }
                else
                {//如果请求的目标是一个小组
                    //获取所有的管理员
                    QVector<GroupMember> members=db.select<GroupMember>( OT(GroupMember::_groupname,OGroupName(request.user)) && OT(GroupMember::_isAdmin,true) );

                    QVectorIterator<GroupMember> i(members);
                    while(i.hasNext())
                    {
                        GroupMember member=i.next();

                        if(OIsOnline(member.uname))
                        {//如果这个管理员在线
                            cl[member.uname]->main->NewGroupRequest(id,request.user,request.uname,request.msg);
                        }
                    }

                }
            }
        }
    }
    else
    {//如果没有指定请求id
        //处理所有未处理的请求
        QVector<UserRequest> requests=db.select<UserRequest>( OT(UserRequest::_isHandle,false) );

        QVectorIterator<UserRequest> i(requests);
        while(i.hasNext())
        {
            processRequest(i.next().id);
        }
    }
}

//public slots:
void OServerCore::log(QString msg)
{
    QString stime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    cout<<QString("%1 > %2").arg(stime).arg(msg)<<endl;
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
            OClientPeer *mainPeer=new OClientPeer(conn);
            client->main=mainPeer;
            connect(client,SIGNAL(lostMainConnect(OClient*)),this,SLOT(onError(OClient*)));
            client->init();
            QString signature=client->getSignature();
            if(cl.contains(signature))
            {
                //这里的情况是，存在与新连接同IP同端口的连接
                //这里显然应该直接销毁旧的连接
                delete cl[signature];
                cl.remove(signature);
            }
            cl.insert(signature,client);
            log(tr("%1 连接到服务器.").arg(signature));
        }
        else
        {
            conn->abort();
            delete conn;
            log(tr("超过了服务器最大客户端上限:%1.").arg(maxClient));
        }
    }
}

void OServerCore::onError(OClient *client)
{
    log(tr("%1 断开连接.").arg(client->getSignature()));

    QString uname=client->getSignature();
    delete client;
    cl.remove(uname);
}
