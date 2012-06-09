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
        QMessageBox::information(0,tr("登录失败"),status);
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
    if(operation==ALL)
    {//如果是请求全部好友
        if(listname==core->uname)
        {//如果是自己的好友列表
            core->mainWidget->ui->friendList->clear();
            QMapIterator<QString,OUserlistItem> i(core->ul);
            while(i.hasNext())
            {
                i.next();
                core->ul.remove(i.key());
                i.next();
            }
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
            core->mainWidget->ui->friendList->clear(true);
            QMapIterator<QString,OUserlistItem> i(core->ul);
            while(i.hasNext())
            {
                i.next();
                if(i.value().status!=OFFLINE)
                {
                    core->ul.remove(i.key());
                    i.next();
                }
            }
        }
        else
        {//如果是某个群的用户列表
            //TODO
        }
    }

    if(operation!=DIFFONLY)
    {//如果是请求全部好友或在线部分好友
        QVectorIterator<OUserlistItem> i(userlist);
        while(i.hasNext())
        {
            OUserlistItem item=i.next();
            core->ul.insert(item.uname,item);
            core->mainWidget->ui->friendList->addItem(item.avatar,item.uname);
            if(item.status!=OFFLINE)
            {
                core->mainWidget->ui->friendList->item(item.uname)->setOnline(true);
            }
        }
    }
}
