#include <QByteArray>
#include <QMessageBox>
#include "const.h"
#include "oclientcoreex.h"
#include "login.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"

//public:
OClientCoreEx::OClientCoreEx():login(0),mainwidget(0)
{

}

OClientCoreEx::~OClientCoreEx()
{

}

void OClientCoreEx::init()
{
    OClientCore::init();
    connect(this,SIGNAL(onTimeChange(uint)),this,SLOT(cbTimeChange(uint)));
    showLogin();
}

void OClientCoreEx::showLogin()
{
    DELETE(login);
    login=new Login;
    connect(this,SIGNAL(onLoginError()),login,SLOT(LoginError()));
    connect(this,SIGNAL(onError(OClientCore::ErrorType,QString,QAbstractSocket::SocketError)),this,SLOT(cbLoginError(OClientCore::ErrorType,QString,QAbstractSocket::SocketError)));
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
    connect(this,SIGNAL(onSMsg(QString,QString,QString)),mainwidget,SLOT(onSMsg(QString,QString,QString)));
    mainwidget->show();
    msgAskUList();
}

void OClientCoreEx::cbLoginError(OClientCore::ErrorType e,QString msg,QAbstractSocket::SocketError s)
{

}

void OClientCoreEx::Error(OClientCore::ErrorType e,QString msg,QAbstractSocket::SocketError s)
{
    OClientCore::Error(e,msg,s);
    QMessageBox::critical(0,tr("错误"),msg);
    if(login)
    {
        login->exitLogin=1;
        login->cancel();
    }
    if(mainwidget)
    {
        DELETE(mainwidget);
        init();
    }
}
