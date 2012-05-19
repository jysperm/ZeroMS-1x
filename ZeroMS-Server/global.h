#ifndef GLOBAL_H
#define GLOBAL_H

#include "../public/OGlobal.h"
#include "../public/ODataBase.h"
#include "OServerDataBaseHeader.h"
class OSettings;
class OServerCore;

const int VERNUM = 2;
const QString VERSION = QObject::trUtf8("2.1.0.%1").arg(::VERNUM);
const QString SERVER = QObject::trUtf8("ZeroMS-Server");
const QString PROTOCOL = QObject::trUtf8("2.0");

extern OSettings *config;
extern OServerCore *core;
extern ODataBase *db;

//检查一个用户名是否在线
//实现在OServerCore.h
inline bool OIsOnline(QString uname);

inline bool OcheckUser(QString uname);//检查一个用户是否存在
inline bool OcheckPWD(QString uname,QString pwdHash,QString publicKey);//检查密码是否正确
inline bool OcheckGroup(QString group);//检查一个小组是否存在
inline bool OcheckGroupMember(QString group,QString uname);//检查uname是否在group小组中

inline bool OcheckUser(QString uname)
{
    using namespace OSDB;
    return !db->selectFrist<User>("uname",uname)._isEmpty;
}

inline bool OcheckPWD(QString uname,QString pwdHash,QString publicKey)
{
    using namespace OSDB;
    if(pwdHash==OSha1(publicKey+db->selectFrist<User>("uname",uname).pwd))
        return true;
    return false;
}

inline bool OcheckGroup(QString group)
{
    using namespace OSDB;
    if(OIsGroup(group))
        group=OGroupName(group);
    return !db->selectFrist<Group>("groupname",group)._isEmpty;
}

inline bool OcheckGroupMember(QString group,QString uname)
{
    using namespace OSDB;
    return !(db->selectFrist<GroupMember>( OQuery("groupname",group) && OQuery("uname",uname) )._isEmpty);
}

#endif // GLOBAL_H
