#ifndef OCLIENTCORE_H
#define OCLIENTCORE_H

#include <QObject>
#include "global.h"
#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include "MainWidget.h"
#include "OServerPeer.h"

class LoginWidget;

class OClientCore:public QObject
{
    Q_OBJECT
public:
    explicit OClientCore();
    ~OClientCore();
    void init();
    void initLoginWidget();
    void initMainWidget();

    QString uname;//用户名
    bool isLoged;//是否已经登录
    bool isShowIp;//是否向其他用户展示IP
    QString status;//在线状态

    QString publicKey;//公钥

    OServerPeer *main;//主连接
    QList<OServerPeer*> subConnList;//次要连接数组
    QVector<int> p2pPorts;
    QMap<QString,QVector<OUserlistItem> > userlistCache;

    LoginWidget *loginWidget;
    MainWidget *mainWidget;
};

#endif // OCLIENTCORE_H
