#ifndef P1_H
#define P1_H

namespace P1
{
//--该文件是1版通讯协议的常量定义文件
const int P_VER = 1;

//头部长度
const int P_HEADLEN	= 16;

//消息编号
const int M_Error	= 0;
const int M_AskTime	= 10;
const int M_Time	= 11;
const int M_Ping	= 20;
const int M_Exit	= 30;
const int M_CMsg	= 100;
const int M_SMsg	= 101;
const int M_Login	= 200;
const int M_LoginOk	= 211;
const int M_LoginError	= 213;
const int M_AskUList	= 300;
const int M_UList	= 301;
const int M_ChangeUList	= 311;

//关于用户验证api的返回信息
const int UAPI_RESULT	    = 0;
const int UAPI_UNAME	    = 1;
const int UAPI_LISTNAME	    = 2;
const int UAPI_CLIENTVER    = 3;
const int UAPI_CLIENTNAME   = 4;

//验证是否兼容该协议版本
inline int checkVer(int n)
{
    if(n==1)
        return 1;
    else
        return 0;
}

}	//namespace P1

#endif // P1_H
