#ifndef OMS_SDK_DEFINE_P2_H
#define OMS_SDK_DEFINE_P2_H

//  http://0-ms.org/wiki/protocol/p2
//零毫秒SB2.x消息常量定义
//该文件仅作示例，请勿在实际开发中使用

namespace P2
{
const int P_VER = 2;

//头部长度
const int P_HEADLEN	= 16;

//消息编号
const int M_Login = 1;
const int M_LoginResult = 2;
const int M_AskInfo = 3;
const int M_Info = 4;
const int M_AskPublicKey = 5;
const int M_PublicKey = 6;
const int M_Unknown = 7;
const int M_Hi = 8;
const int M_UserListChanged = 9;
const int M_AskUserList = 10;
const int M_UserList = 11;
const int M_ModifyUserList = 12;
const int M_SystemMsg = 13;
const int M_State = 14;
const int M_Logout = 15;
const int M_SendMsg = 16;
const int M_NewMsg = 17;
const int M_ImageData = 18;
const int M_AskImageData = 19;
const int M_AskImageInfo = 20;
const int M_ImageInfo = 21;
const int M_UserRequest = 22;
const int M_NewRequest = 23;
const int M_RequestResult = 24;
const int M_ModifyGroup = 25;
const int M_AskUserInfo = 26;
const int M_UserInfo = 27;
const int M_CreateTempGroup = 28;
const int M_FileRequest = 29;
const int M_FileRequestResult = 30;
const int M_FilePartData = 31;
const int M_AskFilePartData = 32;
const int M_AskConfigurationKeyList = 33;
const int M_ConfigurationKeyList = 34;
const int M_AskConfigurationData = 35;
const int M_ConfigurationValue = 36;
const int M_CreateConfigurationData = 37;
const int M_RemoveConfigurationKey = 38;
const int M_ProcessError = 39;
const int M_OK = 40;

inline bool checkVer(int n)
{
    if(n==2)
        return true;
    else
        return false;
}

}       //namespace P2

#endif // OMS_SDK_DEFINE_P2_H
