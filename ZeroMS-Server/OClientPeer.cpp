#include <QStringList>
#include <QDateTime>
#include "OClientPeer.h"
#include "OClient.h"
#include "global.h"
#include "OServerCore.h"

//public:
OClientPeer::OClientPeer(QTcpSocket *connect):OAbstractPeer(ClientPeer,connect)
{

}

void OClientPeer::init()
{
    connect(this,SIGNAL(onLogin(QString,QString,QVector<int>,bool,bool,bool)),this,SLOT(onLogin(QString,QString,QVector<int>,bool,bool,bool)));
    connect(this,SIGNAL(onAskInfo(QStringList)),this,SLOT(onAskInfo(QStringList)));
    connect(this,SIGNAL(onAskPublicKey()),this,SLOT(onAskPublicKey()));
    connect(this,SIGNAL(onAskUserList(QString,QString,bool)),this,SLOT(onAskUserList(QString,QString,bool)));
    connect(this,SIGNAL(onModifyUserList(QString,QString,QString,QString)),this,SLOT(onModifyUserList(QString,QString,QString,QString)));
    connect(this,SIGNAL(onState(QString)),this,SLOT(onState(QString)));
    connect(this,SIGNAL(onLogout()),this,SLOT(onLogout()));
    connect(this,SIGNAL(onSendMsg(QString,QString)),this,SLOT(onSendMsg(QString,QString)));
    connect(this,SIGNAL(onUserRequest(QString,QString)),this,SLOT(onUserRequest(QString,QString)));
    connect(this,SIGNAL(onRequestResult(int,QString)),this,SLOT(onRequestResult(int,QString)));
    connect(this,SIGNAL(onModifyGroup(QString,QString,QStringList)),this,SLOT(onModifyGroup(QString,QString,QStringList)));
    connect(this,SIGNAL(onAskUserInfo(QString,QStringList)),this,SLOT(onAskUserInfo(QString,QStringList)));
    connect(this,SIGNAL(onModifyInfo(QString,QMap<QString,QString>)),this,SLOT(onModifyInfo(QString,QMap<QString,QString>)));
    connect(this,SIGNAL(onOK(QString)),this,SLOT(onOK(QString)));

    OAbstractPeer::init();
}

//public slots:
void OClientPeer::onLogin(QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp)
{
    if(client->isLoged)//如果已经登录了
    {
        LoginResult(ALREADY);
        return;
    }

    //测试用代码
    //if(true)

    if(!publicKey.isEmpty() && OcheckPWD(uname,pwdHash,publicKey))
    {//如果已经申请过公钥，且密码正确
        if(isMain)
        {//如果是主连接
            if(core->cl.contains(uname))
            {//如果连接列表里已经有这个用户名了
                if(isForce)
                {//如果是强制登录
                    delete core->cl[uname];
                    core->cl.remove(uname);
                }
                else
                {//如果不是强制登录
                    LoginResult(ISONLINE,core->cl[uname]->main->conn->peerAddress().toString());
                    return;
                }
            }
            core->cl.remove(client->getSignature());
            core->cl.insert(uname,client);
            client->uname=uname;
            client->isLoged=true;
            client->isShowIp=isShowIp;
            (client->p2pPorts)<<p2pPort;
            LoginResult(Protocol2::OK);
            core->userListChange(uname);
            core->processRequest(uname);
            core->processMsg(uname);
        }
        else
        {//如果是次要连接
            if(core->cl.contains(uname))
            {//如果有同名的主连接
                QString signature=client->getSignature();
                //this->client会在后面被addSubConn()更改，所以在这里备份一下
                OClient *client=this->client;
                core->cl[uname]->addSubConn(this);
                client->main=0;
                delete client;
                core->cl.remove(signature);
                (core->cl[uname]->p2pPorts)<<p2pPort;
                LoginResult(Protocol2::OK);
            }
            else
            {//如果没有同名的主连接
                LoginResult(NOMAIN);
            }
        }
    }
    else
    {//如果没有申请过公钥，或者密码错误
        LoginResult(PWDERR);
    }

    publicKey.clear();
}

void OClientPeer::onAskInfo(QStringList keys)
{
    QMap<QString,QString> result;
    QStringListIterator i(keys);
    while(i.hasNext())
    {
        QString key=i.next();
        if(core->info.contains(key))
        {
            result.insert(key,core->info[key]);
        }
        else
        {
            if(key==TIME)
                result.insert(key,QString::number(QDateTime::currentDateTime().toTime_t()));
            else if(key==UPTIME)
                result.insert(key,QString::number( QDateTime::currentDateTime().toTime_t()-core->upTime ));
        }
    }
    Info(result);
}

void OClientPeer::onAskPublicKey()
{
    QByteArray key;

    //这里的公钥采用15个ascii从32到126的随机字符
    for(int i=0;i<15;i++)
    {
        char c = ( qrand()%(126-32) )+32;
        key.append(QString(c));
    }
    publicKey=QString(key);

    PublicKey(key);
}

void OClientPeer::onAskUserList(QString listname,QString operation,bool isHasAvatar)
{
    if(!this->client->isLoged)
    {//未登录
        ProcessError(NEEDLOGIN);
        return;
    }

    using namespace OSDB;

    QVector<OUserlistItem> allList;
    if(listname!=client->uname)
    {//如果是在请求一个小组的成员列表
        listname=listname.remove(0,1);//移除星号
        if(OcheckGroup(listname) && OcheckGroupMember(listname,client->uname))
        {//如果存在这个小组,且是这个小组的成员
            QVector<GroupMember> memberList=db->select<GroupMember>("groupname",listname);

            QVectorIterator<GroupMember> i(memberList);
            while(i.hasNext())
            {
                QString user=i.next().uname;
                User userInfo=db->selectFrist<User>("uname",user);

                GroupMember groupStatus=db->selectFrist<GroupMember>( OQuery("groupname",listname) && OQuery("uname",user) );

                OUserlistItem item;
                if(!core->cl.contains(user))
                {//如果这个用户不在线
                    if(operation==ONLINE)
                        continue;
                }
                else
                {//如果这个用户在线
                    if(core->cl[user]->isShowIp)
                    {
                        item.ip=core->cl[user]->main->conn->peerAddress().toString();

                        item.p2pPorts=core->cl[user]->p2pPorts;
                    }
                }
                item.uname=user;
                item.status=core->getUserStatus(user);
                QStringList status;
                if(groupStatus.isAdmin)
                    status.append(ADMIN);
                if(groupStatus.isDeny)
                    status.append(DENY);
                item.groupStatus=status.join(",");
                if(isHasAvatar)
                    item.avatar=userInfo.avatar;

                allList.append(item);
            }
        }
        else
        {//如果不存在这个小组，或不是这个小组的成员
            if(OcheckGroup(listname))
            {//如果不是这个小组的成员
                ProcessError(NOTINLIST);
            }
            else
            {//如果这个小组不存在
                ProcessError(NOTEXIST);
            }
            return;
        }
    }
    else
    {//如果是在请求自己的好友列表

        //自己加入的小组
        QVector<GroupMember> groups=db->select<GroupMember>("uname",client->uname);
        QVectorIterator<GroupMember> iGroup(groups);
        while(iGroup.hasNext())
        {
            Group info=db->selectFrist<Group>("groupname",iGroup.next().groupname);
            OUserlistItem item;
            item.uname=QString("*%1,%2").arg(info.groupname).arg(info.caption);
            item.status=ONLINE;
            if(isHasAvatar)
                item.avatar=info.avatar;
            allList.append(item);
        }

        //自己的好友列表
        QVector<OSDB::UserList> userlist=db->select<OSDB::UserList>("uname",client->uname);
        QVectorIterator<OSDB::UserList> iUserlist(userlist);
        while(iUserlist.hasNext())
        {
            OSDB::UserList listItem=iUserlist.next();
            OUserlistItem item;

            User userInfo=db->selectFrist<User>("uname",listItem.user);
            if(!OIsOnline(listItem.user))
            {//如果这个用户不在线
                if(operation==ONLINE)
                    continue;
            }
            else
            {//如果这个用户在线
                if(core->cl[listItem.user]->isShowIp)
                {
                    item.ip=core->cl[listItem.user]->main->conn->peerAddress().toString();

                    QVectorIterator<int> i(core->cl[listItem.user]->p2pPorts);
                    while(i.hasNext())
                        item.p2pPorts.append(i.next());
                }
            }
            item.uname=listItem.user;
            item.status=core->getUserStatus(listItem.user);
            if(isHasAvatar)
                item.avatar=userInfo.avatar;

            allList.append(item);
        }
    }

    QVector<OUserlistItem> *cache=&(client->userlistCache[listname]);

    if(operation==DIFFONLY)
    {
        QVector<OUserlistItem> result;//"仅变动"的结果

        QVectorIterator<OUserlistItem> iAll(allList),iCache(*cache);

        //如果allList中有，而cache中没有，则为新条目，加入result
        while(iAll.hasNext())
        {
            OUserlistItem item=iAll.next();

            if(!item.isInVector(cache))
                result.append(item);
        }

        //如果allList和cache中有同用户名的条目，而其他信息不同，则为修改条目，加入result
        iAll.toFront();
        while(iAll.hasNext())
        {
            OUserlistItem item=iAll.next();

            iCache.toFront();
            while(iCache.hasNext())
            {
                OUserlistItem itemCache=iCache.next();
                if(itemCache.uname==item.uname)
                {//如果用户名相同
                    if(!(itemCache==item))  //没重载!=运算符
                    {//如果其他资料不同
                        result.append(item);
                    }
                    break;
                }
            }
        }

        //如果cache中有，而allList中没有，则说明这个用户下线或被删除
        iCache.toFront();
        while(iCache.hasNext())
        {
            OUserlistItem item=iCache.next();

            if(!item.isInVector(&allList))
            {
                if(OIsGroup(item.uname))
                {//如果是一个小组
                    item.status=REMOVED;
                }
                else if(!db->selectFrist<OSDB::UserList>( OQuery("uname",client->uname) && OQuery("user",item.uname))._isEmpty)
                {//如果是一个用户，且这个用户还在用户列表中
                    item.status=OFFLINE;
                }
                else
                {//如果这个用户已经不在用户列表中了
                    item.status=REMOVED;
                }
                result.append(item);
            }
        }
        *cache=allList;
        UserList(listname,operation,result);
    }
    else
    {
        *cache=allList;
        UserList(listname,operation,allList);
    }
}

void OClientPeer::onModifyUserList(QString listname,QString uname,QString operation,QString message)
{
    using namespace OSDB;

    if(client->isLoged)
    {//如果已经登录
        if(listname==client->uname)
        {//如果是在操作自己的好友列表
            bool isGroup=(uname.left(1)=="*")?true:false;
            if(!isGroup)
            {//如果要操作的目标用户名是用户
                if(OcheckUser(uname))
                {//如果存在这个用户
                    if(operation==ADD)
                    {//如果是添加好友
                        if(db->selectFrist<OSDB::UserList>( OQuery("uname",client->uname) && OQuery("user",uname) )._isEmpty)
                        {//如果他们不存在好友关系
                            OSDB::UserList userlist;
                            userlist.uname=client->uname;
                            userlist.user=uname;
                            db->insert<OSDB::UserList>(userlist);
                            UserListChanged(client->uname);
                        }
                        else
                        {//如果他们已经存在好友关系
                            ProcessError(ALREADYINLIST);
                        }
                    }
                    else
                    {//如果是删除好友
                        if(db->selectFrist<OSDB::UserList>( OQuery("uname",client->uname) && OQuery("user",uname) )._isEmpty)
                        {//如果他们不存在好友关系
                            ProcessError(NOTINLIST);
                        }
                        else
                        {//如果他们存在好友关系
                            db->deleteItem<OSDB::UserList>( OQuery("uname",client->uname) && OQuery("user",uname) );
                            UserListChanged(client->uname);
                        }
                    }
                }
                else
                {//如果不存在这个用户
                    ProcessError(NOTEXIST);
                }
            }
            else
            {//如果要操作的目标用户名是小组
                QString group=OGroupName(uname);//去掉星号后的小组名称
                if(OcheckGroup(group))
                {//如果存在这个小组
                    if(operation==REMOVE)
                    {//如果是退出操作
                        if(!db->selectFrist<GroupMember>( OQuery("groupname",group) && OQuery("uname",client->uname) )._isEmpty)
                        {//如果用户确实在这个小组中
                            db->deleteItem<GroupMember>( OQuery("groupname",group) && OQuery("uname",client->uname) );
                            UserListChanged(client->uname);
                            core->userListChange(uname);
                        }
                        else
                        {//如果用户不在这个小组中
                            ProcessError(NOTINLIST);
                        }
                    }
                    else
                    {//如果是加入操作
                        ProcessError(NOTALLOWTOJOINGROUP);
                    }
                }
                else
                {//如果不存在这个小组
                    ProcessError(NOTEXIST);
                }
            }
        }
        else
        {//如果是在操作某个小组的列表
            QString group=listname.right(listname.length()-1);//去掉星号后的小组名称
            if(OcheckGroup(group))
            {//如果存在这个小组
                GroupMember groupmember=db->selectFrist<GroupMember>( OQuery("groupname",group) && OQuery("uname",client->uname) );
                if(!groupmember._isEmpty && groupmember.isAdmin)
                {//如果用户在这个小组中且有管理员权限
                    if(operation==ADD)
                    {//如果是邀请其他用户加入小组
                        if(OcheckUser(uname))
                        {//如果存在这个用户
                            if(db->selectFrist<GroupMember>( OQuery("groupname",group) && OQuery("uname",uname) )._isEmpty)
                            {//如果这个用户不在这个小组中
                                OSDB::UserRequest request;
                                request.time=QDateTime::currentDateTime().toTime_t();
                                request.uname=listname;
                                request.user=uname;
                                request.invitation=client->uname;
                                request.msg=message;
                                request.isHandle=false;
                                request.handleTime=0;
                                request.result=false;
                                int id=db->insert<OSDB::UserRequest>(request);
                                core->processRequest(id);
                            }
                            else
                            {//如果这个用户已经在这个小组中
                                ProcessError(ALREADYINLIST);
                            }
                        }
                        else
                        {//如果不存在这个用户
                            ProcessError(NOTEXIST);
                        }
                    }
                    else
                    {//如果是从小组中踢除用户
                        if(OcheckUser(uname))
                        {//如果存在这个用户
                            if(!db->selectFrist<GroupMember>( OQuery("groupname",group) && OQuery("uname",uname) )._isEmpty)
                            {//如果这个用户在这个小组中
                                db->deleteItem<GroupMember>( OQuery("groupname",group) && OQuery("uname",uname) );
                                core->userListChange(listname);
                                core->userListChange(uname);
                                //TODO：还要给uname用户发送SystemMsg，通知它已经被踢出group小组
                            }
                            else
                            {//如果这个用户不在这个小组中
                                ProcessError(NOTINLIST);
                            }
                        }
                        else
                        {//如果不存在这个用户
                            ProcessError(NOTEXIST);
                        }
                    }
                }
                else if(groupmember._isEmpty)
                {//如果用户不在这个小组
                    ProcessError(NOTINLIST);
                }
                else
                {//如果用户在这个小组但没有管理员权限
                    ProcessError(NOTADMIN);
                }
            }
            else
            {//如果不存在这个小组
                ProcessError(NOTEXIST);
            }
        }
    }
    else
    {//如果没有登录
        ProcessError(NEEDLOGIN);
    }
}

void OClientPeer::onState(QString status)
{
    if(!this->client->isLoged)
    {//未登录
        ProcessError(NEEDLOGIN);
        return;
    }

    if(client->status!=status)
    {
        client->status=status;
        core->userListChange(client->uname);
    }
}

void OClientPeer::onLogout()
{
    onError();
}

void OClientPeer::onSendMsg(QString uname,QString message)
{
    using namespace OSDB;

    if(!this->client->isLoged)
    {//未登录
        ProcessError(NEEDLOGIN);
        return;
    }

    if(OcheckUser(uname) || OcheckGroup(OGroupName(uname)))
    {//如果存在这个用户或小组
        if(OIsGroup(uname))
        {//如果是往小组发信息
            GroupMember memberInfo=db->selectFrist<GroupMember>( OQuery("groupname",OGroupName(uname)) && OQuery("uname",client->uname) );
            if(memberInfo._isEmpty)
            {//如果自己不在这个小组中
                ProcessError(NOTINLIST);
            }

            if(memberInfo.isDeny)
            {
                ProcessError(NOTADMIN);
            }
        }

        MsgLog msg;
        msg.time=QDateTime::currentDateTime().toTime_t();
        msg.signTime=0;
        msg.uname=client->uname;
        msg.user=uname;
        msg.msg=message;
        msg.isSign=false;
        int id=db->insert<MsgLog>(msg);
        core->processMsg(id);
    }
    else
    {//如果不存在这个用户或小组
        ProcessError(NOTEXIST);
    }
}

void OClientPeer::onUserRequest(QString uname,QString message)
{
    using namespace OSDB;

    if(!this->client->isLoged)
    {//未登录
        ProcessError(NEEDLOGIN);
        return;
    }

    if(!OIsGroup(uname))
    {//如果是在请求一个用户添加自己
        if(OcheckUser(uname))
        {//如果存在这个用户
            if(db->selectFrist<OSDB::UserList>( OQuery("uname",uname) && OQuery("user",client->uname) )._isEmpty)
            {//如果他们之间没有好友关系
                if(db->selectFrist<OSDB::UserRequest>( OQuery("uname",client->uname) &&
                                                       OQuery("user",uname) &&
                                                       OQuery("isHandle",false) )._isEmpty)
                {//没有同样的未处理的请求
                    OSDB::UserRequest request;
                    request.time=QDateTime::currentDateTime().toTime_t();
                    request.uname=client->uname;
                    request.user=uname;
                    request.msg=message;
                    request.isHandle=false;
                    request.handleTime=0;
                    request.result=false;
                    int id=db->insert<OSDB::UserRequest>(request);
                    core->processRequest(id);
                }
                else
                {//有同样的未处理的请求
                    db->update<OSDB::UserRequest>(OQuery("uname",client->uname) &&
                                                  OQuery("user",uname) &&
                                                  OQuery("isHandle",false),
                                                  "msg",message);
                    ProcessError(ALREADYSEND);
                }
            }
            else
            {//如果他们之间已经有好友关系了
                ProcessError(ALREADYINLIST);
            }
        }
        else
        {//如果不存在这个用户
            ProcessError(NOTEXIST);
        }
    }
    else
    {//如果是申请加入小组
        QString group=OGroupName(uname);//去掉星号后的小组名称
        if(OcheckGroup(group))
        {//如果存在这个小组
            if(db->selectFrist<GroupMember>( OQuery("groupname",group) && OQuery("uname",client->uname) )._isEmpty)
            {//如果自己不在这个小组中
                if(db->selectFrist<OSDB::UserRequest>( OQuery("uname",client->uname) &&
                                                       OQuery("user",uname) &&
                                                       OQuery("isHandle",false) )._isEmpty)
                {//没有同样的未处理的请求
                    OSDB::UserRequest request;
                    request.time=QDateTime::currentDateTime().toTime_t();
                    request.uname=client->uname;
                    request.user=uname;
                    request.msg=message;
                    request.isHandle=false;
                    request.handleTime=0;
                    request.result=false;
                    int id=db->insert<OSDB::UserRequest>(request);
                    core->processRequest(id);
                }
                else
                {//有同样的未处理的请求
                    db->update<OSDB::UserRequest>(OQuery("uname",client->uname) &&
                                                  OQuery("user",uname) &&
                                                  OQuery("isHandle",false),
                                                  "msg",message);
                    ProcessError(ALREADYSEND);
                }
            }
            else
            {//如果自己已经在这个小组中
                ProcessError(ALREADYINLIST);
            }
        }
        else
        {//如果不存在这个小组
            ProcessError(NOTEXIST);
        }
    }
}

void OClientPeer::onRequestResult(int id,QString result)
{
    if(!this->client->isLoged)
    {//未登录
        ProcessError(NEEDLOGIN);
        return;
    }

    using namespace OSDB;

    OSDB::UserRequest request=db->selectFrist<OSDB::UserRequest>("id",id);
    if(request._isEmpty)
    {//如果不存在这个请求
        ProcessError(NOTEXIST);
    }
    else
    {//如果存在这个请求
        if(request.user==client->uname)
        {//如果被请求的对象是自己
            if(!request.isHandle)
            {//如果这个请求还没有被处理
                bool r=result==ALLOW?true:false;
                if(OIsGroup(request.uname))
                {//如果是邀请加入小组
                    QString group=OGroupName(request.uname);//去掉星号后的小组名称
                    if(r)
                    {//如果同意了请求
                        if(db->selectFrist<GroupMember>( OQuery("groupname",group) && OQuery("uname",client->uname) )._isEmpty)
                        {//如果自己不在这个小组中
                            GroupMember groupMember;
                            groupMember.groupname=group;
                            groupMember.uname=client->uname;
                            groupMember.isAdmin=false;
                            groupMember.isDeny=false;
                            groupMember.regTime=QDateTime::currentDateTime().toTime_t();
                            db->insert<GroupMember>(groupMember);
                        }
                        else
                        {//如果自己已经在这个小组中
                            ProcessError(ALREADYINLIST);
                        }
                    }
                }
                else
                {//如果是请求对方添加我
                    if(r)
                    {//如果同意了请求
                        if(db->selectFrist<OSDB::UserList>( OQuery("uname",client->uname) && OQuery("user",request.uname) )._isEmpty)
                        {//如果不存在好友关系
                            OSDB::UserList userList;
                            userList.uname=client->uname;
                            userList.user=request.uname;
                            db->insert<OSDB::UserList>(userList);
                        }
                        else
                        {//如果已经存在好友关系
                            ProcessError(ALREADYINLIST);
                        }
                    }
                }

                db->update<OSDB::UserRequest>("id",id,"isHandle",true);
                db->update<OSDB::UserRequest>("id",id,"handleTime",QDateTime::currentDateTime().toTime_t());
                db->update<OSDB::UserRequest>("id",id,"result",r);
            }
            else
            {//如果这个请求已经被处理过了
                ProcessError(ALREADYSEND);
            }
        }
        else
        {//如果被请求的对象不是自己
            ProcessError(NOPERMISSION);
        }

    }
}

void OClientPeer::onModifyGroup(QString group,QString uname,QStringList operators)
{
    if(!this->client->isLoged)
    {//未登录
        ProcessError(NEEDLOGIN);
        return;
    }

    using namespace OSDB;

    if(OcheckGroup(group))
    {//如果存在这个小组
        OQuerys querys=OQuery("groupname",group) && OQuery("uname",uname);
        if(!db->selectFrist<GroupMember>(querys)._isEmpty)
        {//如果被修改的用户在这个群里面
            int rowsAffected=0;
            bool isMaster=(db->selectFrist<Group>("groupname",group).master==client->uname)?true:false;
            bool isAdmin=db->selectFrist<GroupMember>(OQuery("groupname",group) && OQuery("uname",client->uname)).isAdmin;


            QListIterator<QString> i(operators);
            while(i.hasNext())
            {
                QString operation=i.next();
                if(operation==ADMIN)
                {
                    if(isMaster)
                        rowsAffected+=db->update<GroupMember>(querys,"isAdmin",true);
                    else
                        ProcessError(NOTADMIN);
                }
                else if(operation==NOTADMIN)
                {
                    if(isMaster)
                        rowsAffected+=db->update<GroupMember>(querys,"isAdmin",false);
                    else
                        ProcessError(NOTADMIN);
                }
                else if(operation==ALLOW)
                {
                    if(isAdmin)
                        rowsAffected+=db->update<GroupMember>(querys,"isDeny",false);
                    else
                        ProcessError(NOTADMIN);
                }
                else if(operation==DENY)
                {
                    if(isAdmin)
                        rowsAffected+=db->update<GroupMember>(querys,"isDeny",true);
                    else
                        ProcessError(NOTADMIN);
                }
            }

            if(rowsAffected)
            {
                core->userListChange(OToGroup(group));
            }
        }
        else
        {//如果被修改的用户不在这个群里面
            ProcessError(NOTINLIST);
        }
    }
    else
    {//如果不存在这个小组
        ProcessError(NOTEXIST);
    }
}

void OClientPeer::onAskUserInfo(QString uname,QStringList keys)
{
    using namespace OSDB;

    QMap<QString,QString> result;
    bool isGroup=OIsGroup(uname);

    Group groupInfo;
    User userInfo;

    if(isGroup)
    {//如果是在请求一个小组的信息
        if(!OcheckGroup(OGroupName(uname)))
        {//如果不存在这个小组
            ProcessError(NOTEXIST);
            return;
        }
        groupInfo=db->selectFrist<Group>( OQuery("groupname",OGroupName(uname)) );
    }
    else
    {//如果是在请求一个用户的信息
        if(!OcheckUser(uname))
        {//如果不存在这个用户
            ProcessError(NOTEXIST);
            return;
        }
        userInfo=db->selectFrist<User>( OQuery("uname",uname) );
    }

    QListIterator<QString> i(keys);
    while(i.hasNext())
    {
        if(i.next()==STATE)
        {
            if(isGroup)
                continue;
            result.insert(STATE,core->getUserStatus(uname));
        }
        else if(i.peekPrevious()==IP)
        {
            if(isGroup)
                continue;
            if(core->cl.contains(uname) && core->cl[uname]->isShowIp)
                result.insert(IP,core->cl[uname]->main->conn->peerAddress().toString());
        }
        else if(i.peekPrevious()==P2P)
        {
            if(isGroup)
                continue;
            if(core->cl.contains(uname) && core->cl[uname]->isShowIp)
            {
                QString strPorts;
                QVectorIterator<int> i(core->cl[uname]->p2pPorts);
                while(i.hasNext())
                {
                    if(!strPorts.isEmpty())
                        strPorts.append(",");
                    strPorts.append(i.next());
                }
                result.insert(P2P,strPorts);
            }
        }
        else if(i.peekPrevious()==EMAIL)
        {
            if(isGroup)
                continue;
            result.insert(EMAIL,userInfo.email);
        }
        else if(i.peekPrevious()==REGTIME)
        {
            if(isGroup)
                result.insert(REGTIME,QString::number(groupInfo.regTime));
            else
                result.insert(REGTIME,QString::number(userInfo.regTime));
        }
        else if(i.peekPrevious()==CAPTION)
        {
            if(!isGroup)
                continue;
            result.insert(CAPTION,groupInfo.caption);
        }
        else if(i.peekPrevious()==WEBSITE)
        {
            if(isGroup)
                result.insert(WEBSITE,groupInfo.website);
            else
                result.insert(WEBSITE,userInfo.website);
        }
        else if(i.peekPrevious()==ADMINS)
        {
            if(!isGroup)
                continue;
            QString strAdmins;
            QVector<GroupMember> admins=db->select<GroupMember>( OQuery("groupname",OGroupName(uname)) && OQuery("isAdmin",true) );
            QVectorIterator<GroupMember> i(admins);
            while(i.hasNext())
            {
                if(!strAdmins.isEmpty())
                    strAdmins.append(",");
                strAdmins.append(i.next().uname);
            }
            result.insert(ADMINS,strAdmins);
        }
        else if(i.peekPrevious()==MASTER)
        {
            if(!isGroup)
                continue;;
            result.insert(MASTER,groupInfo.master);
        }
        else if(i.peekPrevious()==ONLINETIME)
        {
            if(isGroup)
                continue;
            result.insert(ONLINETIME,QString::number(userInfo.onlineTime));
        }
        else if(i.peekPrevious()==INFO)
        {
            if(isGroup)
                result.insert(INFO,groupInfo.info);
            else
                result.insert(INFO,userInfo.info);
        }
        else if(i.peekPrevious()==AVATAR)
        {
            if(isGroup)
                result.insert(AVATAR,groupInfo.avatar);
            else
                result.insert(AVATAR,userInfo.avatar);
        }
    }

    UserInfo(uname,result);
}

void OClientPeer::onOK(QString id)
{
    if(!this->client->isLoged)
    {//未登录
        ProcessError(NEEDLOGIN);
        return;
    }

    using namespace OSDB;

    MsgLog msg=db->selectFrist<MsgLog>(OQuery("id",id.toInt()));
    if(msg.user==client->uname)
    {
        db->update<MsgLog>(OQuery("id",id.toInt()),"isSign",true);
        db->update<MsgLog>(OQuery("id",id.toInt()),"signTime",QDateTime::currentDateTime().toTime_t());
    }
    else
    {
        ProcessError(NOTADMIN);
    }
}

void OClientPeer::onModifyInfo(QString uname,QMap<QString,QString> values)
{
    if(!this->client->isLoged)
    {//未登录
        ProcessError(NEEDLOGIN);
        return;
    }

    using namespace OSDB;
    bool isGroup=OIsGroup(uname);

    if(!isGroup)
    {//如果修改的目标是一个用户
        if(uname!=client->uname)
        {//如果修改的目标不是自己
            ProcessError(NOTADMIN);
            return;
        }
    }
    else
    {//如果修改的目标是一个小组
        GroupMember memberInfo=db->selectFrist<GroupMember>( OQuery("groupname",OGroupName(uname)) && OQuery("uname",client->uname) );
        if(OcheckGroup(OGroupName(uname)) && !memberInfo._isEmpty && memberInfo.isAdmin)
        {//如果存在这个小组，且自己是小组的管理员

        }
        else
        {//如果不存在这个小组，或自己不是这个小组的管理员
            ProcessError(NOTADMIN);
            return;
        }
    }

    QMapIterator<QString,QString> i(values);
    while(i.hasNext())
    {
        i.next();
        if(i.key()==EMAIL)
        {
            if(isGroup)
                continue;
            db->update<User>(OQuery("uname",uname),"email",i.value());
        }
        else if(i.key()==CAPTION)
        {
            if(!isGroup)
                continue;
            db->update<Group>(OQuery("groupname",OGroupName(uname)),"caption",i.value());
        }
        else if(i.key()==WEBSITE)
        {
            if(isGroup)
                db->update<Group>(OQuery("groupname",OGroupName(uname)),"website",i.value());
            else
                db->update<User>(OQuery("uname",uname),"website",i.value());
        }
        else if(i.key()==INFO)
        {
            if(isGroup)
                db->update<Group>(OQuery("groupname",OGroupName(uname)),"info",i.value());
            else
                db->update<User>(OQuery("uname",uname),"info",i.value());
        }
        else if(i.key()==AVATAR)
        {
            if(isGroup)
                db->update<Group>(OQuery("groupname",OGroupName(uname)),"avatar",i.value());
            else
                db->update<User>(OQuery("uname",uname),"avatar",i.value());
        }
    }
}
