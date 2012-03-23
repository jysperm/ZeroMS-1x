#include "OServerCore.h"
#include <QDateTime>

//private slots:
void OServerCore::Login(OClient::Connect *connect,QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp)
{
    if(connect->client->isLoged)//如果已经登录了
    {
        protocol.LoginResult(connect,ALREADY);
        return;
    }

    //测试用代码
    if((true) || !connect->publicKey.isEmpty() && db.checkPWD(uname,pwdHash,connect->publicKey))
    //if(!connect->publicKey.isEmpty() && db.checkPWD(uname,pwdHash,connect->publicKey))
    {//如果已经申请过公钥，且密码正确
        OClient *client=connect->client;
        if(isMain)
        {//如果是主连接
            if(cl.contains(uname))
            {//如果连接列表里已经有这个用户名了
                if(isForce)
                {//如果是强制登录
                    delete cl[uname];
                    cl.remove(uname);
                }
                else
                {//如果不是强制登录
                    protocol.LoginResult(connect,ISONLINE);
                    return;
                }
            }
            cl.remove(client->getSignature());
            cl.insert(uname,client);
            client->uname=uname;
            client->isLoged=true;
            client->isShowIp=isShowIp;
            (client->p2pPorts)<<p2pPort;
            protocol.LoginResult(connect,OK);
            userListChange(uname);
        }
        else
        {//如果是次要连接
            if(cl.contains(uname))
            {//如果有同名的主连接
                QString signature=connect->client->getSignature();
                connect->client->main=0;
                cl[uname]->addSubConn(connect->conn);
                delete cl[signature];
                cl.remove(signature);
                (cl[uname]->p2pPorts)<<p2pPort;
                protocol.LoginResult(connect,OK);
            }
            else
            {//如果没有同名的主连接
                protocol.LoginResult(connect,NOMAIN);
            }
        }
    }
    else
    {//如果没有申请过公钥，或者密码错误
        protocol.LoginResult(connect,PWDERR);
    }

    connect->publicKey="";
}

void OServerCore::AskUserList(OClient::Connect *connect,QString listname,QString operation,bool isHasAvatar)
{
    OClient *client=connect->client;
    QVector<OClient::UserlistItem> allList;
    if(listname!=client->uname)
    {//如果是在请求一个群的成员列表
        listname=listname.remove(0,1);//移除星号
        if(db.checkGroup(listname) && db.checkGroupMember(listname,client->uname))
        {//如果存在这个群,且是这个群的成员
            QVector<QString> memberList=db.getGroupMembers(listname);

            QVectorIterator<QString> i(memberList);
            while(i.hasNext())
            {
                QString user=i.next();
                ODataBase::UserInfo userInfo=db.getUserInfo(user);
                ODataBase::UserGroupStatus groupStatus=db.getGroupStatus(user,listname);

                OClient::UserlistItem item;
                if(!cl.contains(user))
                {//如果这个用户不在线
                    if(operation==ONLINE)
                        continue;
                }
                else
                {//如果这个用户在线
                    if(cl[user]->isShowIp)
                    {
                        item.ip=cl[user]->main->conn->peerAddress().toString();

                        QVectorIterator<int> i(cl[user]->p2pPorts);
                        while(i.hasNext())
                            item.p2pPorts.append(i.next());
                    }
                }
                item.uname=user;
                item.status=getUserStatus(user);
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
            protocol.Unknown(connect);
            return;
        }
    }
    else
    {//如果是在请求自己的好友列表
        QVector<QString> groups=db.getAllGroup(client->uname);
        QVectorIterator<QString> iGroup(groups);
        while(iGroup.hasNext())
        {
            ODataBase::GroupInfo info=db.getGroupInfo(iGroup.next());
            OClient::UserlistItem item;
            item.uname=QString("*%1,%2").arg(info.groupname).arg(info.caption);
            item.status=ONLINE;
            if(isHasAvatar)
                item.avatar=info.avatar;
            allList.append(item);
        }

        QVector<ODataBase::UserListItem> userlist=db.getUserList(client->uname);
        QVectorIterator<ODataBase::UserListItem> iUserlist(userlist);
        while(iUserlist.hasNext())
        {
            ODataBase::UserListItem listItem=iUserlist.next();
            OClient::UserlistItem item;
            ODataBase::UserInfo userInfo=db.getUserInfo(listItem.user);
            if(!cl.contains(listItem.user))
            {//如果这个用户不在线
                if(operation==ONLINE)
                    continue;
            }
            else
            {//如果这个用户在线
                if(cl[listItem.user]->isShowIp)
                {
                    item.ip=cl[listItem.user]->main->conn->peerAddress().toString();

                    QVectorIterator<int> i(cl[listItem.user]->p2pPorts);
                    while(i.hasNext())
                        item.p2pPorts.append(i.next());
                }
            }
            item.uname=listItem.user;
            item.status=getUserStatus(listItem.user);
            if(isHasAvatar)
                item.avatar=userInfo.avatar;

            allList.append(item);
        }
    }

    QVector<OClient::UserlistItem> *cache=&(connect->client->userlistCache[listname]);

    if(operation==DIFFONLY)
    {
        QVector<OClient::UserlistItem> result;

        QVectorIterator<OClient::UserlistItem> iAll(allList),iCache(*cache);
        while(iAll.hasNext())
        {
            OClient::UserlistItem item=iAll.next();

            if(!iCache.findNext(item))
                result.append(item);
            iCache.toFront();
        }

        while(iCache.hasNext())
        {
            OClient::UserlistItem item=iCache.next();

            iAll.toFront();
            if(!iAll.findNext(item))
            {
                if(item.uname.left(1)=="*")
                {//如果是一个群
                    item.status=REMOVED;
                }
                else if(!db.getUserList(client->uname,item.uname).isEmpty())
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
        protocol.UserList(connect,listname,operation,result);
    }

    *cache=allList;

    protocol.UserList(connect,listname,operation,allList);
}

void OServerCore::ModifyUserList(OClient::Connect *connect,QString listname,QString uname, bool isAddOrRemove, QString message)
{
    if(connect->client->isLoged)
    {//如果已经登录
        if(listname==connect->client->uname)
        {
            bool isGroup=(uname.left(1)=="*")?true:false;
            if(isAddOrRemove && !isGroup)
            {//如果是添加用户
                if(db.checkUser(uname))
                {//如果这个用户存在
                    db.ModifyUserList(connect->client->uname,uname,true);
                }
            }
            else if(!isGroup)
            {//如果是删除用户
                db.ModifyUserList(connect->client->uname,uname,false);
            }
            else
            {//如果是删除群
                db.removeGroupMember(uname,connect->client->uname);
            }
        }
        else
        {
            if(db.checkGroup(listname))
            {

            }
            else
            {
                protocol.Unknown(connect);
            }
        }
    }
    else
    {//如果没有登录
        protocol.Unknown(connect);
    }
}

void OServerCore::AskInfo(OClient::Connect *connect,QStringList keys)
{
    QMap<QString,QString> result;
    QStringListIterator i(keys);
    while(i.hasNext())
    {
        QString key=i.next();
        if(info.contains(key))
        {
            result.insert(key,info[key]);
        }
        else
        {
            if(key==TIME)
                result.insert(key,QString::number(QDateTime::currentDateTime().toTime_t()));
        }
    }
    protocol.Info(connect,result);
}

void OServerCore::State(OClient::Connect *connect,QString status)
{
    if(connect->client->status!=status)
    {
        connect->client->status=status;
        userListChange(connect->client->uname);
    }
}


void OServerCore::AskPublicKey(OClient::Connect *connect)
{
    QByteArray key;

    //这里的公钥采用15个ascii从32到126的随机字符
    for(int i=0;i<15;i++)
    {
        char c = ( qrand()%(126-32) )+32;
        key.append(QString(c));
    }
    connect->publicKey=QString(key);

    protocol.PublicKey(connect,key);
    log(QString("%1 请求公钥").arg(connect->client->getSignature()));
}
