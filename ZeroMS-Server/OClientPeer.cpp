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
    connect(this,SIGNAL(Login(QString,QString,QVector<int>,bool,bool,bool)),this,SLOT(onLogin(QString,QString,QVector<int>,bool,bool,bool)));
    connect(this,SIGNAL(AskPublicKey()),this,SLOT(onAskPublicKey()));
    connect(this,SIGNAL(State(QString)),this,SLOT(onState(QString)));
    connect(this,SIGNAL(AskInfo(QStringList)),this,SLOT(onAskInfo(QStringList)));
    connect(this,SIGNAL(AskUserList(QString,QString,bool)),this,SLOT(onAskUserList(QString,QString,bool)));
    connect(this,SIGNAL(ModifyUserList(QString,QString,QString,QString)),this,SLOT(onModifyUserList(QString,QString,QString,QString)));
    connect(this,SIGNAL(UserRequest(QString,QString)),this,SLOT(onUserRequest(QString,QString)));
    connect(this,SIGNAL(RequestResult(int,QString)),this,SLOT(onRequestResult(int,QString)));

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
    if(true)
        //if(!publicKey.isEmpty() && core->db.checkPWD(uname,pwdHash,publicKey))
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
            LoginResult(OK);
            core->userListChange(uname);
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
                LoginResult(OK);
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
    {//如果是在请求一个群的成员列表
        listname=listname.remove(0,1);//移除星号
        if(core->db.checkGroup(listname) && core->db.checkGroupMember(listname,client->uname))
        {//如果存在这个群,且是这个群的成员
            QVector<GroupMember> memberList=core->db.select<GroupMember>(OT(GroupMember::_groupname,listname));

            QVectorIterator<GroupMember> i(memberList);
            while(i.hasNext())
            {
                QString user=i.next().uname;
                User userInfo=core->db.selectFrist<User>(OT(User::_uname,user));

                GroupMember groupStatus=core->db.selectFrist<GroupMember>( OT(GroupMember::_groupname,listname) && OT(GroupMember::_uname,user) );

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

                        QVectorIterator<int> i(core->cl[user]->p2pPorts);
                        while(i.hasNext())
                            item.p2pPorts.append(i.next());
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
        {//如果不存在这个群，或不是这个群的成员
            if(core->db.checkGroup(listname))
            {//如果不是这个群的成员
                ProcessError(NOTINLIST);
            }
            else
            {//如果这个群不存在
                ProcessError(NOTEXIST);
            }
            return;
        }
    }
    else
    {//如果是在请求自己的好友列表
        QVector<GroupMember> groups=core->db.select<GroupMember>(OT(GroupMember::_uname,client->uname));
        QVectorIterator<GroupMember> iGroup(groups);
        while(iGroup.hasNext())
        {
            Group info=core->db.selectFrist<Group>(OT(Group::_groupname,iGroup.next().groupname));
            OUserlistItem item;
            item.uname=QString("*%1,%2").arg(info.groupname).arg(info.caption);
            item.status=ONLINE;
            if(isHasAvatar)
                item.avatar=info.avatar;
            allList.append(item);
        }

        QVector<OSDB::UserList> userlist=core->db.select<OSDB::UserList>(OT(OSDB::UserList::_uname,client->uname));
        QVectorIterator<OSDB::UserList> iUserlist(userlist);
        while(iUserlist.hasNext())
        {
            OSDB::UserList listItem=iUserlist.next();
            OUserlistItem item;

            User userInfo=core->db.selectFrist<User>(OT(User::_uname,listItem.user));
            if(!core->cl.contains(listItem.user))
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
        QVector<OUserlistItem> result;

        QVectorIterator<OUserlistItem> iAll(allList),iCache(*cache);
        while(iAll.hasNext())
        {
            OUserlistItem item=iAll.next();

            if(!iCache.findNext(item))
                result.append(item);
            iCache.toFront();
        }

        while(iCache.hasNext())
        {
            OUserlistItem item=iCache.next();

            iAll.toFront();
            if(!iAll.findNext(item))
            {
                if(item.uname.left(1)=="*")
                {//如果是一个群
                    item.status=REMOVED;
                }
                else if(!core->db.selectFrist<OSDB::UserList>( OT(OSDB::UserList::_uname,client->uname) && OT(OSDB::UserList::_user,item.uname))._isEmpty)
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
        UserList(listname,operation,result);
    }

    *cache=allList;

    UserList(listname,operation,allList);
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
                if(core->db.checkUser(uname))
                {//如果存在这个用户
                    if(operation==ADD)
                    {//如果是添加好友
                        if(core->db.selectFrist<OSDB::UserList>( OT(OSDB::UserList::_uname,client->uname) && OT(OSDB::UserList::_user,uname) )._isEmpty)
                        {//如果他们不存在好友关系
                            OSDB::UserList userlist;
                            userlist.uname=client->uname;
                            userlist.user=uname;
                            core->db.insert<OSDB::UserList>(userlist);
                            UserListChanged(client->uname);
                        }
                        else
                        {//如果他们已经存在好友关系
                            ProcessError(ALREADYINLIST);
                        }
                    }
                    else
                    {//如果是删除好友
                        if(core->db.selectFrist<OSDB::UserList>( OT(OSDB::UserList::_uname,client->uname) && OT(OSDB::UserList::_user,uname) )._isEmpty)
                        {//如果他们不存在好友关系
                            ProcessError(NOTINLIST);
                        }
                        else
                        {//如果他们存在好友关系
                            core->db.deleteItem<OSDB::UserList>( OT(OSDB::UserList::_uname,client->uname) && OT(OSDB::UserList::_user,uname) );
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
                QString group=uname.right(uname.length()-1);//去掉星号后的小组名称
                if(core->db.checkGroup(group))
                {//如果存在这个小组
                    if(operation==REMOVE)
                    {//如果是退出操作
                        if(!core->db.selectFrist<GroupMember>( OT(GroupMember::_groupname,group) && OT(GroupMember::_uname,client->uname) )._isEmpty)
                        {//如果用户确实在这个小组中
                            core->db.deleteItem<GroupMember>( OT(GroupMember::_groupname,group) && OT(GroupMember::_uname,client->uname) );
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
            if(core->db.checkGroup(group))
            {//如果存在这个小组
                GroupMember groupmember=core->db.selectFrist<GroupMember>( OT(GroupMember::_groupname,group) && OT(GroupMember::_uname,client->uname) );
                if(!groupmember._isEmpty && groupmember.isAdmin)
                {//如果用户在这个小组中且有管理员权限
                    if(operation==ADD)
                    {//如果是邀请其他用户加入小组
                        if(core->db.checkUser(uname))
                        {//如果存在这个用户
                            if(core->db.selectFrist<GroupMember>( OT(GroupMember::_groupname,group) && OT(GroupMember::_uname,uname) )._isEmpty)
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
                                int id=core->db.insert<OSDB::UserRequest>(request);
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
                        if(core->db.checkUser(uname))
                        {//如果存在这个用户
                            if(!core->db.selectFrist<GroupMember>( OT(GroupMember::_groupname,group) && OT(GroupMember::_uname,uname) )._isEmpty)
                            {//如果这个用户在这个小组中
                                core->db.deleteItem<GroupMember>( OT(GroupMember::_groupname,group) && OT(GroupMember::_uname,uname) );
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

void OClientPeer::onUserRequest(QString uname,QString message)
{
    using namespace OSDB;

    if(!this->client->isLoged)
    {//未登录
        ProcessError(NEEDLOGIN);
        return;
    }

    bool isGroup=(uname.left(1)=="*")?true:false;
    if(!isGroup)
    {//如果是在请求一个用户添加自己
        if(core->db.checkUser(uname))
        {//如果存在这个用户
            if(core->db.selectFrist<OSDB::UserList>( OT(OSDB::UserList::_uname,uname) && OT(OSDB::UserList::_user,client->uname) )._isEmpty)
            {//如果他们之间没有好友关系
                if(core->db.selectFrist<OSDB::UserRequest>( OT(OSDB::UserRequest::_uname,client->uname) &&
                                                            OT(OSDB::UserRequest::_user,uname) &&
                                                            OT(OSDB::UserRequest::_isHandle,false) )._isEmpty)
                {//没有同样的未处理的请求
                    OSDB::UserRequest request;
                    request.time=QDateTime::currentDateTime().toTime_t();
                    request.uname=client->uname;
                    request.user=uname;
                    request.msg=message;
                    request.isHandle=false;
                    request.handleTime=0;
                    request.result=false;
                    int id=core->db.insert<OSDB::UserRequest>(request);
                    core->processRequest(id);
                }
                else
                {//有同样的未处理的请求
                    core->db.update<OSDB::UserRequest>(OT(OSDB::UserRequest::_uname,client->uname) &&
                                                       OT(OSDB::UserRequest::_user,uname) &&
                                                       OT(OSDB::UserRequest::_isHandle,false) ,
                                                       OSDB::UserRequest::_msg,message);
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
        QString group=uname.right(uname.length()-1);//去掉星号后的小组名称
        if(core->db.checkGroup(group))
        {//如果存在这个小组
            if(core->db.selectFrist<GroupMember>( OT(GroupMember::_groupname,group) && OT(GroupMember::_uname,client->uname) )._isEmpty)
            {//如果自己不在这个小组中
                if(core->db.selectFrist<OSDB::UserRequest>( OT(OSDB::UserRequest::_uname,client->uname) &&
                                                            OT(OSDB::UserRequest::_user,uname) &&
                                                            OT(OSDB::UserRequest::_isHandle,false) )._isEmpty)
                {//没有同样的未处理的请求
                    OSDB::UserRequest request;
                    request.time=QDateTime::currentDateTime().toTime_t();
                    request.uname=client->uname;
                    request.user=uname;
                    request.msg=message;
                    request.isHandle=false;
                    request.handleTime=0;
                    request.result=false;
                    int id=core->db.insert<OSDB::UserRequest>(request);
                    core->processRequest(id);
                }
                else
                {//有同样的未处理的请求
                    core->db.update<OSDB::UserRequest>(OT(OSDB::UserRequest::_uname,client->uname) &&
                                                       OT(OSDB::UserRequest::_user,uname) &&
                                                       OT(OSDB::UserRequest::_isHandle,false) ,
                                                       OSDB::UserRequest::_msg,message);
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

    OSDB::UserRequest request=core->db.selectFrist<OSDB::UserRequest>( OT(OSDB::UserRequest::_id,id) );
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
                if(request.uname.left(1)=="*")
                {//如果是邀请加入小组
                    QString group=request.uname.right(request.uname.length()-1);//去掉星号后的小组名称
                    if(r)
                    {//如果同意了请求
                        if(core->db.selectFrist<GroupMember>( OT(GroupMember::_groupname,group) && OT(GroupMember::_uname,client->uname) )._isEmpty)
                        {//如果自己不在这个小组中
                            GroupMember groupMember;
                            groupMember.groupname=group;
                            groupMember.uname=client->uname;
                            groupMember.isAdmin=false;
                            groupMember.isDeny=false;
                            groupMember.regTime=QDateTime::currentDateTime().toTime_t();
                            core->db.insert<GroupMember>(groupMember);
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
                        if(core->db.selectFrist<OSDB::UserList>( OT(OSDB::UserList::_uname,client->uname) && OT(OSDB::UserList::_user,request.uname) )._isEmpty)
                        {//如果不存在好友关系
                            OSDB::UserList userList;
                            userList.uname=client->uname;
                            userList.user=request.uname;
                            core->db.insert<OSDB::UserList>(userList);
                        }
                        else
                        {//如果已经存在好友关系
                            ProcessError(ALREADYINLIST);
                        }
                    }
                }

                core->db.update<OSDB::UserRequest>(OT(OSDB::UserRequest::_id,id),OSDB::UserRequest::_isHandle,true);
                core->db.update<OSDB::UserRequest>(OT(OSDB::UserRequest::_id,id),OSDB::UserRequest::_handleTime,QDateTime::currentDateTime().toTime_t());
                core->db.update<OSDB::UserRequest>(OT(OSDB::UserRequest::_id,id),OSDB::UserRequest::_result,r);
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




