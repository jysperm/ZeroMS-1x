#include "OServerCore.h"
#include <QDateTime>

//private slots:
void OServerCore::Login(OClient::Connect *connect,QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp)
{
    qDebug()<<Q_FUNC_INFO;

    if(connect->client->isLoged)//如果已经登录了
        protocol.LoginResult(connect,ALREADY);

    if(!connect->publicKey.isEmpty() && db.checkPWD(uname,pwdHash,connect->publicKey))
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

void OServerCore::ModifyUserList(OClient::Connect *connect,QString uname,bool isAddOrRemove)
{
    if(connect->client->isLoged)
    {//如果已经登录
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

    protocol.Unknown(connect);
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
