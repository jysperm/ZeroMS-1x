#ifndef CLIENTCOREEX_H
#define CLIENTCOREEX_H

#include <QMap>
#include <QDir>
#include "const.h"
#include "oclientcore.h"
#include "chatwidget.h"
#include "../public/osettings.h"
#include "../public/odowner.h"
class QApplication;
class Login;
class MainWidget;

class OClientCoreEx:public OClientCore
{
    Q_OBJECT
    //该类用意就在连接GUI与ClientCore，为方便GUI存取数据，全部成员公有
public:
//重写基类的函数:
    OClientCoreEx();
    ~OClientCoreEx();
    void init();

    void msgLoginOk(QByteArray *data,unsigned int time);
    void msgUList(QByteArray *data,unsigned int time);
    void Error(OClientCore::ErrorType e,QString msg,QAbstractSocket::SocketError s);

//新函数:
    void showLogin();//显示登陆窗口
    void showMainWidget();//显示主窗口
    void showChatWidget(QString uname);
    void writeChatLog(QString user,QString msg);

    //工具函数，建立path所在的目录(path是一个文件的路径)
    inline void mkDir(QString path);

    Login *login;
    MainWidget *mainwidget;
    //已经打开的聊天窗口列表
    QMap<QString,ChatWidget*> widgets;
    //配置文件读取器
    OSettings config;
    ODowner downer;
public slots:
    //这里为了省去解析消息，没有重载函数，而是使用槽函数接收信号
    void onMsg(QString user,QString view,QString msg);
    //当widgets中的ChatWidget销毁时发射onDelete信号,被该槽接收，用于把自身从widgets中删除
    void removeFromWidgets(QString uname);
    //downer发出的下载完成
    void onDownFinish(ODowner::FileAddress file);
signals:
    void onGroupMsg(QString uname,QString msg);
};

inline void OClientCoreEx::mkDir(QString path)
{
    QDir().mkpath(QFileInfo(path).dir().path());
}

#endif // CLIENTCOREEX_H
