#ifndef PUBLIC_OGLOBAL_PROTOCOL2_0_H
#define PUBLIC_OGLOBAL_PROTOCOL2_0_H

namespace Protocol2
{
const int P_VER = 2;

//头部长度
const int P_HEADLEN	= 16;

//消息编号
const int Login         = 1;
const int LoginResult	= 2;
const int AskInfo	= 3;
const int Info          = 4;
const int AskPublicKey	= 5;
const int PublicKey	= 6;
const int Unknown	= 7;
const int Hi            = 8;
const int UserListChanged= 9;
const int AskUserList	= 10;
const int UserList	= 11;
const int ModifyUserList= 12;
const int SystemMsg	= 13;
const int State         = 14;
const int Logout	= 15;
const int SendMsg	= 16;
const int NewMsg	= 17;
const int ImageData	= 18;
const int AskImageData  = 19;
const int AskImageInfo  = 20;
const int ImageInfo     = 21;
const int UserRequest   = 22;
const int NewRequest    = 23;
const int RequestResult     = 24;
const int ModifyGroup       = 25;
const int AskUserInfo       = 26;
const int UserInfo          = 27;
const int CreateTempGroup   = 28;
const int FileRequest       = 29;
const int FileRequestResult = 30;
const int FilePartData      = 31;
const int AskFilePartData   = 32;
const int AskConfigurationKeyList   = 33;
const int ConfigurationKeyList      = 34;
const int AskConfigurationData      = 35;
const int ConfigurationValue        = 36;
const int CreateConfigurationData   = 37;
const int RemoveConfigurationKey    = 38;

inline bool checkVer(int n)
{
    if(n==2)
        return true;
    else
        return false;
}

}       //namespace Protocol2

#endif // PUBLIC_OGLOBAL_PROTOCOL2_0_H
