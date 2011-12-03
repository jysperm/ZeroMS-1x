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

//最大客户端连接数量
const int CLIENT_MAX = 100;

//客户端多长时间不发送消息判定为掉线，单位秒
const int Time_OffLine = 200;

//用于登陆的api接口文件
const QString LOGIN_APIURL = "http://jybox.net/0-ms/api/login.php?mode=post";

//与API接口通讯的私钥
const QString API_KEY = "0-ms-0.1";

//无法解析协议时向客户端发出的消息
const QString ERROR_STR = QString::fromUtf8("服务器无法解析您所使用的协议，稍后将断开连接，请到以下地址查看最新通讯协议的有关信息：http://0-ms.org/wiki\n");

#endif // CONST_H
