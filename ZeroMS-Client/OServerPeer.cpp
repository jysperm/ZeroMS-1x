#include "OServerPeer.h"
#include "OClientCore.h"
#include <QMessageBox>
#include "ui_MainWidget.h"
#include "FriendListItem.h"

OServerPeer::OServerPeer(QTcpSocket *connect):OAbstractPeer(ServerPeer,connect)
{

}

void OServerPeer::init()
{
    connect(this,SIGNAL(onLoginResult(QString,QString)),this,SLOT(onLoginResult(QString,QString)));
    connect(this,SIGNAL(onPublicKey(QString)),this,SLOT(onPublicKey(QString)));
    connect(this,SIGNAL(onUserListChanged(QString)),this,SLOT(onUserListChanged(QString)));
    connect(this,SIGNAL(onUserList(QString,QString,QVector<OUserlistItem>)),this,SLOT(onUserList(QString,QString,QVector<OUserlistItem>)));

    OAbstractPeer::init();
}

void OServerPeer::onLoginResult(QString status,QString ip)
{
    if(status==OK)
    {
        core->initMainWidget();
    }
    else
    {
        if(core->loginWidget)
            core->loginWidget->destroyLink();
        QMessageBox::critical(core->loginWidget,tr("登录失败"),status);
    }
}

void OServerPeer::onPublicKey(QString publicKey)
{
    core->publicKey=publicKey;
}

void OServerPeer::onUserListChanged(QString listname)
{
    AskUserList(listname,DIFFONLY,false);
}

void OServerPeer::onUserList(QString listname,QString operation,const QVector<OUserlistItem> &userlist)
{
    FriendList *friendList=core->mainWidget->ui->friendList;

    //先按照情况删除好友
    if(operation==ALL)
    {//如果是请求全部好友
        if(listname==core->uname)
        {//如果是自己的好友列表
            friendList->clear();
        }
        else
        {//如果是某个群的用户列表
            //TODO
        }
    }
    else
    {//如果是只请求在线好友
        if(listname==core->uname)
        {//如果是自己的好友列表
            friendList->clear(true);
        }
        else
        {//如果是某个群的用户列表
            //TODO
        }
    }

    //添加好友到好友列表
    if(operation!=DIFFONLY)
    {//如果是请求全部好友或在线部分好友
        QVectorIterator<OUserlistItem> i(userlist);
        while(i.hasNext())
        {
            OUserlistItem item=i.next();
            friendList->addItem(item);
        }
    }
    else
    {//如果是仅请求变动部分的好友列表
        QVectorIterator<OUserlistItem> i(userlist);
        while(i.hasNext())
        {
            OUserlistItem item=i.next();
            if(friendList->isHas(item.uname))
            {//如果好友列表中已有这个用户
                if(item.status==REMOVED)
                    friendList->removeItem(item.uname);

                if(!item.status.isEmpty())
                    friendList->item(item.uname)->setStatus(item.status);
                if(!item.ip.isEmpty())
                    friendList->item(item.uname)->setIp(item.ip);
                if(!item.p2pPorts.isEmpty())
                    friendList->item(item.uname)->setP2pPorts(item.p2pPorts);
                if(!item.avatar.isEmpty())
                    friendList->item(item.uname)->setAvatar(item.avatar);
            }
            else
            {//如果好友列表中没有这个用户
                friendList->addItem(item);
            }
        }
    }
}
