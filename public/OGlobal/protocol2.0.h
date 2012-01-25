#ifndef PUBLIC_OGLOBAL_PROTOCOL2_0_H
#define PUBLIC_OGLOBAL_PROTOCOL2_0_H

namespace Protocol2
{
const int P_VER = 2;

//头部长度
const int P_HEADLEN	= 16;

//消息编号
const int M_Login       = 1;
const int M_LoginResult	= 2;
const int M_AskInfo     = 3;
const int M_Info        = 4;
const int M_AskPublicKey= 5;
const int M_PublicKey	= 6;
const int M_Unknown	= 7;
const int M_Hi          = 8;
const int M_UserListChanged= 9;
const int M_AskUserList	= 10;
const int M_UserList	= 11;
const int M_ModifyUserList= 12;
const int M_SystemMsg	= 13;
const int M_State       = 14;
const int M_Logout	= 15;
const int M_SendMsg	= 16;
const int M_NewMsg	= 17;
const int M_ImageData	= 18;
const int M_AskImageData  = 19;
const int M_AskImageInfo  = 20;
const int M_ImageInfo     = 21;
const int M_UserRequest   = 22;
const int M_NewRequest    = 23;
const int M_RequestResult     = 24;
const int M_ModifyGroup       = 25;
const int M_AskUserInfo       = 26;
const int M_UserInfo          = 27;
const int M_CreateTempGroup   = 28;
const int M_FileRequest       = 29;
const int M_FileRequestResult = 30;
const int M_FilePartData      = 31;
const int M_AskFilePartData   = 32;
const int M_AskConfigurationKeyList   = 33;
const int M_ConfigurationKeyList      = 34;
const int M_AskConfigurationData      = 35;
const int M_ConfigurationValue        = 36;
const int M_CreateConfigurationData   = 37;
const int M_RemoveConfigurationKey    = 38;

inline bool checkVer(int n)
{
    if(n==2)
        return true;
    else
        return false;
}

}       //namespace Protocol2

#endif // PUBLIC_OGLOBAL_PROTOCOL2_0_H
