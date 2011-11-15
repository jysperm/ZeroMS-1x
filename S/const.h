#ifndef CONST_H
#define CONST_H

#include <QString>
#include <QObject>
//该文件是全局常量定义文件，其他所有.h文件和main.cpp统一包含该文件，无论是否用到
//ps.使用const常量会经过C++的类型检查！尽量减少使用预处理宏

//所必须的前置声明,尽量减少包含，尽可能使用前置声明
//尤其不应当包含该工程中的文件
class ClientConn;

typedef int qint32;

//SocketList最大连接数量
const int SocketList_MAX = 100;

//客户端监听端口
const int ServerCore_CLIENT_PORT = 4321;

//SockeList中储存的连接对象的类型
typedef ClientConn ClientConn;

//无法解析协议时向客户端发出的消息
const QString ERROR_STR = QObject::trUtf8("服务器无法解析您所使用的协议，稍后将断开连接，请到以下地址查看最新通讯协议的有关信息：http://code.google.com/p/zeroms/\n");

//用于登陆的api接口文件
const QString LOGIN_APIURL = "http://jybox.net/0-ms/api/login.php?mode=post";
//const QString LOGIN_APIURL = "http://jybox.net/cs.php";

//判断版本是否支持
inline int checkVer(int n)
{
    if(n==1)
        return 1;
    else
        return 0;
}

//与API接口通讯的私钥
const QString API_KEY = "0-ms-0.1";

//关于用户验证api的返回信息
const int UAPI_RESULT = 0;
const int UAPI_UNAME = 1;
const int UAPI_LISTNAME = 2;
const int UAPI_CLIENTVER = 3;
const int UAPI_CLIENTNAME = 3;

//-------------------------------------------------
//翻译

//对QAbstractSocket::SocketState枚举的翻译
//qthelp://com.trolltech.qt.472/qdoc/qabstractsocket.html#SocketState-enum
const QString SocketState_CN[7]={"连接没有被打开",
                                 "正在查找目标主机",
                                 "正在建立连接",
                                 "连接已建立",
                                 "连接已经绑定了IP和端口",
                                 "未知状态(该状态编号被QT预留)",
                                 "连接即将被关闭"};

//对QAbstractSocket::SocketError枚举的翻译
//qthelp://com.trolltech.qt.472/qdoc/qabstractsocket.html#SocketError-enum
inline QString SocketError_CN(int i)
{
    const QString SocketError_CN_ex[20]={"未定义的错误",
                                         "连接被对方拒绝或超时",
                                         "远程主机关闭了连接",
                                         "主机地址没有被找到",
                                         "没有权限来执行这个操作",
                                         "本地端口资源不足",
                                         "操作超时",
                                         "报文长度超过操作系统的限制",
                                         "网络发生错误(例如网线被断开)",
                                         "该地址(端口)已经被使用",
                                         "这个地址不属于该主机",
                                         "请求的操作不被操作系统支持(例如IPv6)",
                                         "该连接使用了一个需要认证的代理",
                                         "安全连接握手失败，连接被关闭",
                                         "上一个操作仍在后台进行中",
                                         "因为被拒绝，无法连接代理服务器",
                                         "到代理服务器的连接被意外中断(连接已经成功)",
                                         "在连接到代理服务器进行认证时服务器超时或停止相应",
                                         "没有发现代理地址",
                                         "正在与代理服务器进行协商，因为对方无法解析"};
    return SocketError_CN_ex[i+1];
}

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
