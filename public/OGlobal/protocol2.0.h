#ifndef PUBLIC_OGLOBAL_PROTOCOL2_0_H
#define PUBLIC_OGLOBAL_PROTOCOL2_0_H

class QString;

namespace Protocol2
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

const QString OK = "ok";
const QString PWDERR = "pwderr";
const QString ISONLINE = "isonline";
const QString UNKNOWN = "unknown";
const QString NOMAIN = "nomain";
const QString ALREADY = "already";
const QString OFFLINECACHETIME = "offlinecachetime";
const QString NOACTIVITYTIME = "noactivitytime";
const QString VERSION = "version";
const QString PROTOCOL = "protocol";
const QString TIME = "time";
const QString VERNUM = "vernum";
const QString WEBSITE = "website";
const QString SUB = "sub";
const QString FORCE = "force";
const QString HIDEIP = "hideip";
const QString ADD = "add";
const QString REMOVE = "remove";
const QString ALL = "all";
const QString ONLINE = "online";
const QString DIFFONLY = "diffonly";
const QString AVATAR = "avatar";
const QString NOAVATAR = "noavatar";
const QString BORED = "bored";
const QString BUZY = "buzy";
const QString AWAY = "away";
const QString OFFLINE = "offline";
const QString REMOVED = "removed";
const QString ADMIN = "admin";
const QString NOTADMIN = "notadmin";
const QString ALLOW = "allow";
const QString DENY = "deny";

const QString NEEDLOGIN = "needlogin";
const QString ALREADYINLIST = "alreadyinlist";
const QString NOTINLIST = "notinlist";
const QString NOTEXIST = "notexist";

inline bool checkVer(int n)
{
    if(n==2)
        return true;
    else
        return false;
}

inline QString num2String(int type)
{
    //该函数提供从枚举值到字符串的转换
    switch(type)
    {
        case 1:
            return "M_Login";
        case 2:
            return "M_LoginResult";
        case 3:
            return "M_AskInfo";
        case 4:
            return "M_Info";
        case 5:
            return "M_AskPublicKey";
        case 6:
            return "M_PublicKey";
        case 7:
            return "M_Unknown";
        case 8:
            return "M_Hi";
        case 9:
            return "M_UserListChanged";
        case 10:
            return "M_AskUserList";
        case 11:
            return "M_UserList";
        case 12:
            return "M_ModifyUserList";
        case 13:
            return "M_SystemMsg";
        case 14:
            return "M_State";
        case 15:
            return "M_Logout";
        case 16:
            return "M_SendMsg";
        case 17:
            return "M_NewMsg";
        case 18:
            return "M_ImageData";
        case 19:
            return "M_AskImageData";
        case 20:
            return "M_AskImageInfo";
        case 21:
            return "M_ImageInfo";
        case 22:
            return "M_UserRequest";
        case 23:
            return "M_NewRequest";
        case 24:
            return "M_RequestResult";
        case 25:
            return "M_ModifyGroup";
        case 26:
            return "M_AskUserInfo";
        case 27:
            return "M_UserInfo";
        case 28:
            return "M_CreateTempGroup";
        case 29:
            return "M_FileRequest";
        case 30:
            return "M_FileRequestResult";
        case 31:
            return "M_FilePartData";
        case 32:
            return "M_AskFilePartData";
        case 33:
            return "M_AskConfigurationKeyList";
        case 34:
            return "M_ConfigurationKeyList";
        case 35:
            return "M_AskConfigurationData";
        case 36:
            return "M_ConfigurationValue";
        case 37:
            return "M_CreateConfigurationData";
        case 38:
            return "M_RemoveConfigurationKey";
        case 39:
            return "M_ProcessError";
        case 40:
            return "M_OK";
        default:
            return "";
    }
}

}       //namespace Protocol2

#endif // PUBLIC_OGLOBAL_PROTOCOL2_0_H
