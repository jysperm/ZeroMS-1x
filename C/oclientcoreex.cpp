#include <QByteArray>
#include "const.h"
#include "oclientcoreex.h"
#include "login.h"
#include "mainwidget.h"

//public:
OClientCoreEx::OClientCoreEx():login(0)
{

}

OClientCoreEx::~OClientCoreEx()
{

}

void OClientCoreEx::init()
{
    connect(this,SIGNAL(onTimeChange(uint)),this,SLOT(cbTimeChange(uint)));
    login=new Login;
    connect(this,SIGNAL(onError(OClientCore::ErrorType,QString,QAbstractSocket::SocketError)),login,SLOT(socketError(OClientCore::ErrorType,QString,QAbstractSocket::SocketError)));
    connect(this,SIGNAL(onLoginError()),login,SLOT(LoginError()));
    login->show();
}

void OClientCoreEx::cbTimeChange(unsigned int time)
{
    //cb为回调的意思,为了放置和基类重名，只好用这个蛋疼的前缀
    login->timeuped=1;
}

void OClientCoreEx::msgLoginOk(QByteArray *data,unsigned int time)
{
    DELETE(login);
    mainwidget=new MainWidget;
    connect(this,SIGNAL(onUList(QStringList&)),mainwidget,SLOT(onUList(QStringList&)));
    mainwidget->show();
    msgAskUList();
}

