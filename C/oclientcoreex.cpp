#include <QApplication>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include "const.h"
#include "inline.h"
#include "oclientcoreex.h"
#include "ui_mainwidget.h"
#include "login.h"
#include "mainwidget.h"
#include "threadlogodowner.h"

//临时
#include <QMessageBox>

//public:
OClientCoreEx::OClientCoreEx(QApplication *app):ClientCore(app),login(0),mw(0),error(0)
{

}

OClientCoreEx::~OClientCoreEx()
{
    sDelete(login);
    sDelete(mw);
}

void OClientCoreEx::init()
{
    ClientCore::init();
    login=new Login;
    login->show();
}

//----
void OClientCoreEx::msgLoginOk(QByteArray *data,unsigned int time)
{
    ClientCore::msgLoginOk(data,time);
    sDelete(login);
    mw=new MainWidget;
    mw->show();
    msgAskUList();
}

void OClientCoreEx::msgLoginError(QByteArray *data,unsigned int time)
{
    ClientCore::msgLoginError(data,time);
    login->LoginError();
    throwError();
}

void OClientCoreEx::msgUList(QByteArray *data,unsigned int time)
{
    ClientCore::msgUList(data,time);
    QStringList needDown,users=QString(*data).split(",");

    //临时
    QMessageBox::about(0,0,users.join(" "));

    QString i;
/*
    QDir dir(QDir::currentPath());
    dir.mkdir("cahce");
    dir.cd(QDir::currentPath()+"/cahce");
    dir.mkdir("logo");

    foreach(i,users)
	if(!QFile::exists((QDir::currentPath()+"/cahce/logo/%1.png").arg(i)))
		needDown.append(i);
    downer=new ThreadLogoDowner(needDown,app);
    connect(downer,SIGNAL(finishedDown()),mw,SLOT(logoChange()));
    downer->start();
*/
    //添加在线列表
    mw->ui->UListWidget->clear();
    foreach(i,users)
    {
	mw->ui->UListWidget->addItem(i);
    }
}

//----
void OClientCoreEx::throwError()
{
    ClientCore::throwError();
    error=1;
    if(login)
	login->cancel();
    if(mw)
    {
	sDelete(mw);
	init();
    }
}


