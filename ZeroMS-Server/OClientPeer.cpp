#include "OClientPeer.h"
#include "OServerCore.h"

void OClientPeer::onLogin(QString uname,QString pwdHash,QVector<int> p2pPort,bool isMain,bool isForce,bool isShowIp)
{
    if(client->isLoged)//如果已经登录了
    {
        LoginResult(ALREADY);
        return;
    }

    //测试用代码
    //if(true)
    if(!publicKey.isEmpty() && servercore->db.checkPWD(uname,pwdHash,publicKey))
    {//如果已经申请过公钥，且密码正确
        if(isMain)
        {//如果是主连接
            if(servercore->cl.contains(uname))
            {//如果连接列表里已经有这个用户名了
                if(isForce)
                {//如果是强制登录
                    delete servercore->cl[uname];
                    servercore->cl.remove(uname);
                }
                else
                {//如果不是强制登录
                    LoginResult(connect,ISONLINE,servercore->cl[uname]->main->conn->peerAddress().toString());
                    return;
                }
            }
            servercore->cl.remove(client->getSignature());
            servercore->cl.insert(uname,client);
            client->uname=uname;
            client->isLoged=true;
            client->isShowIp=isShowIp;
            (client->p2pPorts)<<p2pPort;
            LoginResult(connect,OK);
            servercore->userListChange(uname);
        }
        else
        {//如果是次要连接
            if(servercore->cl.contains(uname))
            {//如果有同名的主连接
                QString signature=client->getSignature();
                servercore->cl[uname]->addSubConn(this);
                client->main=0;
                delete client;
                servercore->cl.remove(signature);
                (servercore->cl[uname]->p2pPorts)<<p2pPort;
                LoginResult(connect,OK);
            }
            else
            {//如果没有同名的主连接
                LoginResult(connect,NOMAIN);
            }
        }
    }
    else
    {//如果没有申请过公钥，或者密码错误
        protocol.LoginResult(connect,PWDERR);
    }

    publicKey="";
}
