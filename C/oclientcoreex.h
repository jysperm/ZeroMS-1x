#ifndef CLIENTCOREEX_H
#define CLIENTCOREEX_H

#include "const.h"
#include "oclientcore.h"
class QApplication;
class Login;
class MainWidget;
class ThreadLogoDowner;

class OClientCoreEx:public OClientCore
{
    Q_OBJECT
    //该类用意就在连接GUI与ClientCore，为方便GUI存取数据，全部成员公有
public:
    //重写基类的函数
    explicit OClientCoreEx(QApplication *app=0);
    ~OClientCoreEx();
    void init();

    void msgLoginOk(QByteArray *data,unsigned int time);
    void msgLoginError(QByteArray *data,unsigned int time);
    void msgUList(QByteArray *data,unsigned int time);

    void throwError();

    Login *login;
    MainWidget *mw;
    ThreadLogoDowner *downer;
    //是否出错
    //TODO,又一个蛋疼的设计
    int error;
};

#endif // CLIENTCOREEX_H
