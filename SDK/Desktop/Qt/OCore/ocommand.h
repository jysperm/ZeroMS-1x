#ifndef OCOMMAND_H
#define OCOMMAND_H

#include "OCore_global.h"

OMS_NAMESPACE_BEGIN

class OCORESHARED_EXPORT Command
{
public:
    enum Command_t {
        CT_R_ERROR      =   0,      // 命令无法解析
        CT_S_AckTime    =   10,     // 请求服务器时间
        CT_R_TIME       =   11,     // 服务器时间
        CT_S_Keep       =   20,     // 保持登录
        CT_S_Exit       =   30,     // 退出登录
        CT_S_Msg        =   100,    // 客户端发送信息
        CT_R_Msg        =   101,    // 接收服务端信息
        CT_S_Login      =   200,    // 请求登录
        CT_R_LoginOk    =   211,    // 登录成功
        CT_R_LoginError =   213,    // 登陆失败
        CT_S_AckUList   =   300,    // 请求在线列表
        CT_R_UList      =   301,    // 在线列表
        CT_R_UListChanged=  311     // 在线列表更新
    };
    typedef enum Command_t CT;

    Command(CT c_type);
};

OMS_NAMESPACE_END

#endif // OCOMMAND_H
