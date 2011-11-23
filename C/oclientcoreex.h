#ifndef CLIENTCOREEX_H
#define CLIENTCOREEX_H

#include "const.h"
#include "oclientcore.h"
class QApplication;
class Login;

class OClientCoreEx:public OClientCore
{
    Q_OBJECT
    //该类用意就在连接GUI与ClientCore，为方便GUI存取数据，全部成员公有
public:
    //重写基类的函数
    OClientCoreEx();
    ~OClientCoreEx();
    void init();

    Login *login;
public slots:
    void cbTimeChange(unsigned int time);
};

#endif // CLIENTCOREEX_H
