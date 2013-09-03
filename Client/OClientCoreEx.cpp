#include "const.h"
#include "OClientCoreEx.h"
#include "Login.h"
#include "MainWidget.h"
#include "ui_MainWidget.h"

//public:
OClientCoreEx::OClientCoreEx():login(0),mainwidget(0),config("./config.ini")
{
    //下载器
    downer.isAutoExit=0;
    connect(&downer,SIGNAL(finish(ODownloader::FileAddress)),this,SLOT(onDownFinish(ODownloader::FileAddress)));
    downer.start();
}

OClientCoreEx::~OClientCoreEx()
{

}

void OClientCoreEx::init()
{
    OClientCore::init();

    //删除头像缓存
    if(config["REFRESH_LOGO_ON_START"].toInt())
    {
        QDir logoDir(QFileInfo(config["LOGO_CACHE_PATH"].toString().arg("0")).absolutePath());
        QStringList list=logoDir.entryList(QDir::Files);
        for(QStringList::Iterator i=list.begin();i!=list.end();i++)
        {
            QFile(logoDir.absoluteFilePath(*i)).remove();
        }

    }

    timeOffLine=config["TIME_OFFLINE"].toInt();
    connect(this,SIGNAL(onSMsg(QString,QString,QString)),this,SLOT(onMsg(QString,QString,QString)));
    showLogin();
}

void OClientCoreEx::showLogin()
{
    //调用该函数之前应该检查登陆窗口是否已经显示
    if(login)login->disconnect();
    delete login;
    login=new Login;
    connect(this,SIGNAL(onLoginError()),login,SLOT(LoginError()));
    connect(login,SIGNAL(destroyed()),qApp,SLOT(quit()));
    login->show();
}

void OClientCoreEx::showMainWidget()
{
    //调用该函数之前应该检查主窗口是否已经显示
    if(login) login->disconnect();
    delete login;
    login = 0;
    delete mainwidget;
    mainwidget=new MainWidget;
    connect(this,SIGNAL(onGroupMsg(QString,QString)),mainwidget,SLOT(onMsg(QString,QString)));
    connect(this,SIGNAL(onUList(QStringList&)),mainwidget,SLOT(onUList(QStringList&)));
    mainwidget->show();
}

void OClientCoreEx::showChatWidget(QString uname)
{
    if(widgets.contains(uname))
    {
        widgets[uname]->activateWindow();
    }
    else
    {
        ChatWidget *cp=new ChatWidget(uname);
        connect(cp,SIGNAL(onDelete(QString)),this,SLOT(removeFromWidgets(QString)));
        widgets.insert(uname,cp);
        cp->show();
    }
}

void OClientCoreEx::writeChatLog(QString user,QString msg)
{
    if(config["OPEN_CHATLOG"].toInt())
    {
        QString fileName=config["CHATLOG_PATH"].toString().arg(myname).arg(QString(user).replace("*","_"));

        //这里是创建文件夹..灰常蛋痛，看了半个小时Qt文档也就是这样了....
        mkDir(fileName);

        QFile chatLog(fileName);
        chatLog.open(QFile::Append);
        QByteArray bMsg;
        QString stime=QDateTime::currentDateTime().toString(config["CHATLOG_DATETIME_FORMAT"].toString());
        bMsg.append(config["CHATLOG_FORMAT"].toString().arg(stime).arg(user).arg(msg));
        chatLog.write(bMsg);
        chatLog.close();
    }
}

void OClientCoreEx::debugOut(OPacket packet)
{
    QString stime=QDateTime::currentDateTime().toString(config["DEBUG_OUT_DATETIME_FORMAT"].toString());
    QString text(packet.type%2?tr("Server "):tr("Client "));
    text.append(config["DEBUG_OUT_FORMAT"].toString().arg(stime).arg(P_VER).arg(packet.type).arg(packet.data.size()).arg(QString(packet.data)));
    debugOut(text);
}

void OClientCoreEx::debugOut(QString text)
{
    if(config["OPEN_DEBUG_OUT"].toInt())
    {
        mkDir(config["DEBUG_OUT_PATH"].toString());
        QFile file(config["DEBUG_OUT_PATH"].toString());
        file.open(QFile::Append);
        QByteArray bMsg;
        bMsg.append(text);
        file.write(bMsg);
        file.close();
    }
}

void OClientCoreEx::msgLoginOk(OPacket &packet)
{
    login->onLoginOK();//TODO,这里可以改为信号槽来实现
    showMainWidget();
    msgAskUList();
}

void OClientCoreEx::msgUList(OPacket &packet)
{
    QStringList users=QString(packet.data).split(",");
    foreach(QString i,users)
    {
        if(!QFile::exists(config["LOGO_CACHE_PATH"].toString().arg(i)) && config["OPEN_LOGO_DOWNLOAD"].toInt())
        {
            downer.addFile(ODownloader::FileAddress(config["LOGO_DOWNLOAD_URL"].toString().arg(i),config["LOGO_CACHE_PATH"].toString().arg(i),i));
        }
    }
}

void OClientCoreEx::Error(OClientCore::ErrorType e,QString msg,QAbstractSocket::SocketError s)
{
    OClientCore::Error(e,msg,s);
    QString msgStr;
    switch(e)
    {
        case Unknown:
        case NoError:
            msgStr=tr("未知错误");
            break;
        case CantUnderstand:
            msgStr=tr("无法理解服务器发来的命令，可能是您的客户端版本过旧");
            break;
        case MsgError:
            msgStr=msg;
            break;
        case SocketCantConnect:
            msgStr=tr("无法连接到服务器，有可能是服务器挂掉了...");
            break;
        case SocketConnectionAbort:
            msgStr=tr("连接被服务器中断");
            break;
        case SocketOthers:
            msgStr=msg;
    }
    QMessageBox::critical(0,tr("错误 %1").arg(e),msgStr);
    if(e>3 && !login)
    {
        //如果是Socket错误，而且当前不在登陆窗体
        //则销毁其他所有窗体，并显示登陆窗体
        delete mainwidget;
        mainwidget = 0;

        for(QMap<QString,ChatWidget*>::Iterator i=widgets.begin();i!=widgets.end();++i)
        {
            delete i.value();
            widgets.erase(i);
        }

        isLoged=0;
        showLogin();
    }
}

void OClientCoreEx::sendPacket(OPacket &packet)
{
    OClientCore::sendPacket(packet);
    debugOut(packet);
}

void OClientCoreEx::receivePacket(OPacket &packet)
{
    OClientCore::receivePacket(packet);
    debugOut(packet);
}

void OClientCoreEx::onMsg(QString user,QString view,QString msg)
{
    if(view==MAIN_GROUP)
    {
        emit onGroupMsg(user,msg);
    }
    else
    {
        showChatWidget(user);
        widgets[user]->onMsg(msg);
        widgets[user]->activateWindow();
        qApp->alert(widgets[user]);
    }

    writeChatLog(view,msg);
}

void OClientCoreEx::removeFromWidgets(QString uname)
{
    widgets.remove(uname);
}

void OClientCoreEx::onDownFinish(ODownloader::FileAddress file)
{
    if(file.key==myname && mainwidget)
        mainwidget->logoChange();

    if(widgets.contains(file.key))
        widgets[file.key]->logoChange();
}
