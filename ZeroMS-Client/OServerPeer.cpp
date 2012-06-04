#include "OServerPeer.h"
#include "OClientCore.h"
#include <QMessageBox>

OServerPeer::OServerPeer(QTcpSocket *connect):OAbstractPeer(ServerPeer,connect)
{

}

void OServerPeer::init()
{
    connect(this,SIGNAL(onLoginResult(QString,QString)),this,SLOT(onLoginResult(QString,QString)));
    connect(this,SIGNAL(onPublicKey(QString)),this,SLOT(onPublicKey(QString)));

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
