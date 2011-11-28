#ifndef CONST_H
#define CONST_H

//该文件是全局常量定义文件，其他所有文件统一包含该文件，无论是否用到
//ps.使用const常量会经过C++的类型检查！尽量减少使用预处理宏

//尽量减少包含头文件，尽可能使用前置声明
#include <QObject>
#include <QString>
#include "p1.h"
using namespace P1;

//该宏几乎每个文件都要用到，考虑性能问题，不放在inline.h里
//注意，如果要操作的指针是只读的，不要使用这个宏，直接用delete关键字就行
#define DELETE(p) if(p)delete (p);(p)=0;

//客户端名称字符串,用于窗口标题
//因为到这里，main函数还没被执行，没有设置默认编码,所以要用trUtf8
const QString CLIENT_TITLE_NAME = QObject::trUtf8("零毫秒");
//客户的名称字符串，用于通知服务器
const QString CLIENT_NAME = QObject::trUtf8("零毫秒官方客户端");
//版本顺序号
const int CLIENT_VER_NUM = 2;
//版本描述性字符串(每次编译发布版的时候再修改就可以)
const QString CLIENT_VER_NAME = QObject::trUtf8("0.0.2.%1-2011.11.28 bbf9517e23147b82e3333646626f297229fc493c").arg(CLIENT_VER_NUM);

//注册帐号的网址
const QString REG_URL = "http://jybox.net/bbs/register.php";

//找回密码的网址
const QString FORGET_URL = "http://jybox.net/bbs/sendpwd.php";

//服务器ip和端口
const QString SERVER_ADDRESS = "173.212.235.252";
//const QString SERVER_ADDRESS = "127.0.0.1";
const int SERVER_PORT = 4321;

//主群群号
const QString MAIN_GROUP = "*0";

#endif // CONST_H
