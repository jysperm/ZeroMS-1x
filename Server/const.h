#ifndef CONST_H
#define CONST_H

//该文件是全局常量定义文件，其他所有文件统一包含该文件，无论是否用到
//ps.使用const常量会经过C++的类型检查！尽量减少使用预处理宏

//尽量减少包含头文件，尽可能使用前置声明
#include <QtCore>
#include <QtNetwork>
#include "../public/p1.h"
using namespace P1;

//版本描述性字符串(每次编译发布版的时候再修改就可以)
const QString SERVER_VER_NAME = QObject::trUtf8("%1.2.0-2013.9.3 0000000000").arg(P_VER);

#endif // CONST_H
