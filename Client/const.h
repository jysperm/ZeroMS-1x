#ifndef CONST_H
#define CONST_H

#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtWidgets>
#include "../public/p1.h"
using namespace P1;

//客户端名称字符串,用于窗口标题
//因为到这里，main函数还没被执行，没有设置默认编码,所以要用trUtf8
const QString CLIENT_TITLE_NAME = QObject::trUtf8("零毫秒");
//客户的名称字符串，用于通知服务器
const QString CLIENT_NAME = QObject::trUtf8("零毫秒官方客户端");

//版本顺序号
const int CLIENT_VER_NUM = 8;
//版本描述性字符串(每次编译发布版的时候再修改就可以)
const QString CLIENT_VER_NAME = QObject::trUtf8("1.1.0 build%1-2013.9.3 862df94734").arg(CLIENT_VER_NUM);

#endif // CONST_H
