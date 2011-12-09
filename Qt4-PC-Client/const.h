#ifndef CONST_H
#define CONST_H

//该文件是全局常量定义文件，其他所有文件统一包含该文件，无论是否用到
//ps.使用const常量会经过C++的类型检查！尽量减少使用预处理宏

//尽量减少包含头文件，尽可能使用前置声明
#include <QObject>
#include <QString>
#include "../public/const.h"
#include "../public/p1.h"
using namespace P1;

//客户端名称字符串,用于窗口标题
//因为到这里，main函数还没被执行，没有设置默认编码,所以要用trUtf8
const QString CLIENT_TITLE_NAME = QObject::trUtf8("零毫秒");
//客户的名称字符串，用于通知服务器
const QString CLIENT_NAME = QObject::trUtf8("零毫秒官方客户端");

//版本顺序号
const int CLIENT_VER_NUM = 2;
//版本描述性字符串(每次编译发布版的时候再修改就可以)
const QString CLIENT_VER_NAME = QObject::trUtf8("0.0.2.%1-2011.11.28 6388987f75").arg(CLIENT_VER_NUM);

#endif // CONST_H
