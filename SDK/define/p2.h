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
const int State     = 13;
const int Logout	= 14;
const int SendMsg	= 15;
const int NewMsg	= 16;
const int ImageData	= 17;
const int AskImageData  = 18;
const int AskImageInfo  = 19;
const int ImageInfo     = 20;
const int UserRequest   = 21;
const int NewRequest    = 22;
const int RequestResult     = 23;
const int ModifyGroup    	= 24;
const int AskUserInfo       = 25;
const int UserInfo          = 26;
const int CreateTempGroup   = 27;
const int FileRequest       = 28;
const int FileRequestResult = 29;
const int FilePartData      = 30;
const int AskFilePartData   = 31;
const int AskConfigurationKeyList   = 32;
const int ConfigurationKeyList      = 33;
const int AskConfigurationData      = 34;
const int ConfigurationValue        = 35;
const int CreateConfigurationData   = 36;
const int RemoveConfigurationKey    = 37;

inline bool checkVer(int n)
{
    if(n==2)
        return true;
    else
        return false;
}

}       //namespace P2

#endif // OMS_SDK_DEFINE_P2_H
