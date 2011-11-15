#ifndef CONST_H
#define CONST_H

#include <QString>
#include <QObject>
//该文件是全局常量定义文件，所有文件统一包含该文件，无论是否用到
//宏和inline函数请写道inline.h,这里只写常量定义
//ps.使用const会经过C++的类型检查！尽量减少使用预处理宏

//客户端名称字符串,用于窗口标题
//因为到这里，main函数还没被执行，没有设置默认编码
const QString CLIENT_TITLE_NAME = QObject::trUtf8("零毫秒");
//客户的名称字符串，用于通知服务器
const QString CLIENT_NAME = QObject::trUtf8("零毫秒官方客户端");
//版本顺序号
const int CLIENT_VER_NUM = 1;
//版本描述性字符串
const QString CLIENT_VER_NAME = QObject::trUtf8("0.0.1.%1-2011.10.30 r40").arg(CLIENT_VER_NUM);

//注册帐号的网址
const QString REG_URL = "http://jybox.net/bbs/register.php";

//找回密码的网址
const QString FORGET_URL = "http://jybox.net/bbs/sendpwd.php";

//服务器ip和端口
//const QString SERVER_ADDRESS = "173.212.235.252";
const QString SERVER_ADDRESS = "127.0.0.1";
const int SERVER_PORT = 4321;

//正在使用的协议
const int P_USE = 1;

//-------------------------------------------------
//协议相关 版本1

//头部长度
const int P1_HEAD_LEN = 16;
//用于waitForLen，代表等待一个头
const int P1_HEAD = -1;

const int M1_Error = 0;
const int M1_AskTime = 10;
const int M1_Time = 11;
const int M1_Ping = 20;
const int M1_Exit = 30;
const int M1_CMsg = 100;
const int M1_SMsg = 101;
const int M1_Login = 200;
const int M1_LoginOk = 211;
const int M1_LoginError = 213;
const int M1_AskUList = 300;
const int M1_UList = 301;
const int M1_ChangeUList = 311;

#endif // CONST_H
