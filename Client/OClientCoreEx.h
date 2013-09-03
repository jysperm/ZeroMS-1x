#ifndef CLIENTCOREEX_H
#define CLIENTCOREEX_H

#include "const.h"
#include "OClientCore.h"
#include "ChatWidget.h"
#include "../public/OSettings.h"
#include "../public/ODownloader.h"
class Login;
class MainWidget;

class OClientCoreEx : public OClientCore
{
    Q_OBJECT
    //该类用意就在连接GUI与ClientCore，为方便GUI存取数据，全部成员公有
public:
//重写基类的函数:
    OClientCoreEx();
    ~OClientCoreEx();
    void init();

    void msgLoginOk(OPacket &packet);
    void msgUList(OPacket &packet);
    void Error(OClientCore::ErrorType e,QString msg,QAbstractSocket::SocketError s);
    void sendPacket(OPacket &packet);
    void receivePacket(OPacket &packet);

//新函数:
    void showLogin();//显示登陆窗口
    void showMainWidget();//显示主窗口
    void showChatWidget(QString uname);//显示私聊窗体
    void writeChatLog(QString user,QString msg);//聊天记录

    //调试输出
    void debugOut(OPacket packet);
    void debugOut(QString text);

    //工具函数，建立path所在的目录(path是一个文件的路径)
    inline void mkDir(QString path);

    Login *login;
    MainWidget *mainwidget;
    //已经打开的聊天窗口列表
    QMap<QString,ChatWidget*> widgets;
    //配置文件读取器
    OSettings config;
    ODownloader downer;
public slots:
    //这里为了省去解析消息，没有重载函数，而是使用槽函数接收信号
    void onMsg(QString user,QString view,QString msg);
    //当widgets中的ChatWidget销毁时发射onDelete信号,被该槽接收，用于把自身从widgets中删除
    void removeFromWidgets(QString uname);
    //downer发出的下载完成
    void onDownFinish(ODownloader::FileAddress file);
signals:
    void onGroupMsg(QString uname,QString msg);
};

inline void OClientCoreEx::mkDir(QString path)
{
    QDir().mkpath(QFileInfo(path).dir().path());
}

#endif // CLIENTCOREEX_H
