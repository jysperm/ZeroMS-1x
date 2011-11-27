#include <QByteArray>
#include <QMessageBox>
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
    OClientCore::init();
    connect(this,SIGNAL(onTimeChange(uint)),this,SLOT(cbTimeChange(uint)));
    login=new Login;
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

