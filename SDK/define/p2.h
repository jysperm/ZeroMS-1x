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
const int Login	= 1;
const int LoginResult	= 2;
const int AskInfo	= 3;
const int Info	= 4;
const int AskPublicKey	= 5;
const int PublicKey	= 6;
const int Unknown	= 7;
const int Hi    = 8;
const int UserListChanged= 9;
const int AskUserList	= 10;
const int UserList	= 11;
const int SystemMsg	= 12;
const int Logout	= 13;
const int SendMsg	= 14;
const int NewMsg	= 15;
const int ImageData	= 16;
const int AskImageData  = 17;
const int AskImageInfo  = 18;
const int ImageInfo     = 19;
const int UserRequest   = 20;
const int NewRequest    = 21;
const int NewGroupRequest   = 22;
const int RequestResult     = 23;
const int AskUserInfo       = 24;
const int UserInfo          = 25;
const int CreateTempGroup   = 26;
const int FileRequest       = 27;
const int FileRequestResult = 28;
const int FilePartData      = 29;
const int AskFilePartData   = 30;
const int AskConfigurationKeyList   = 31;
const int ConfigurationKeyList      = 32;
const int AskConfigurationData      = 33;
const int ConfigurationValue        = 34;
const int CreateConfigurationData   = 35;
const int RemoveConfigurationKey    = 36;

inline bool checkVer(int n)
{
    if(n==2)
        return true;
    else
        return false;
}

}       //namespace P2

#endif // OMS_SDK_DEFINE_P2_H
