#ifndef ODATABASE_H
#define ODATABASE_H

#include <QDebug>
#include <QSqlQuery>
#include "OClient.h"
#include "OServerDataBaseHeader.h"

namespace OSDB
{

class Querys;

class OT
{
public:
    OT();
    OT(QString k,QString v);

    inline OSDB::Querys operator && (OSDB::OT t);
    inline OSDB::Querys operator || (OSDB::OT t);

    QString k;
    QString v;
};

class Querys
{
public:
    Querys();
    Querys(OT t);

    inline OSDB::Querys operator && (OSDB::OT t);
    inline OSDB::Querys operator || (OSDB::OT t);

    QString getSQL();

    QString sql;
};

}       //namespace OSDB

class OServerDataBase
{
public:
    OServerDataBase();
    ~OServerDataBase();

    template<class T> T selectFrist(OSDB::Querys query=OSDB::Querys(),QString order="",bool isASC=true);
    template<class T> QVector<T> selectTable(OSDB::Querys query=OSDB::Querys(),QString order="",int start=-1,int num=-1,bool isASC=true);

    template<class T> int update(T source,T target);
    template<class T> int update(OSDB::Querys querys,T target);
    template<class T> int update(OSDB::Querys querys,QString field,QVariant value);

    inline bool checkUser(QString uname);//检查一个用户是否存在
    inline bool checkPWD(QString uname,QString pwd,QString publicKey);//检查密码是否正确
    inline bool checkGroup(QString group);//检查一个小组是否存在
    inline bool checkGroupMember(QString group,QString uname);//检查uname是否在group小组中

    /*
    void removeGroupMember(QString group,QString uname);
    void ModifyUserList(QString uname,QString user,bool isAddOrRemove);
    QVector<QString> getAllGroup(QString uname=QString());
    QVector<QString> getGroupMembers(QString group);
    OGroup getGroupInfo(QString group);
    OUser getUserInfo(QString uname);
    OGroupMember getGroupStatus(QString uname,QString group);
    QVector<OUserList> getUserList(QString uname,QString user=QString());
    QVector<OUserList> getUserListByUser(QString user);*/
private:
    QSqlDatabase *dbConn;
};

inline OSDB::Querys OSDB::OT::operator && (OSDB::OT t)
{
    OSDB::Querys querys(*this);
    return (querys && t);
}

inline OSDB::Querys OSDB::OT::operator || (OSDB::OT t)
{
    OSDB::Querys querys(*this);
    return (querys || t);
}

inline OSDB::Querys OSDB::Querys::operator && (OSDB::OT t)
{
    QString expr=QString("( `%1` = '%2' )").arg(t.k).arg(t.v.replace("'","\\'"));;

    if(sql.isEmpty())
        sql=expr;
    else
        sql=QString("( %1 AND %2 )").arg(sql).arg(expr);

    return *this;
}

inline OSDB::Querys OSDB::Querys::operator || (OSDB::OT t)
{
    QString expr=QString("( `%1` = '%2' )").arg(t.k).arg(t.v.replace("'","\\'"));;

    if(sql.isEmpty())
        sql=expr;
    else
        sql=QString("( %1 OR %2 )").arg(sql).arg(expr);

    return *this;
}

inline bool OServerDataBase::checkUser(QString uname)
{
    using namespace OSDB;
    return !selectFrist<User>(OT(User::_uname,uname)).isEmpty;
}

inline bool OServerDataBase::checkPWD(QString uname,QString pwd,QString publicKey)
{
    using namespace OSDB;
    if(pwd==OSha1(publicKey+selectFrist<User>(OT(User::_uname,uname)).pwd))
        return true;
    return false;
}

inline bool OServerDataBase::checkGroup(QString group)
{
    using namespace OSDB;
    return !selectFrist<Group>(OT(Group::_groupname,group)).isEmpty;
}

inline bool OServerDataBase::checkGroupMember(QString group,QString uname)
{
    using namespace OSDB;
    return !selectFrist<GroupMember>( OT(GroupMember::_groupname,group) && OT(GroupMember::_uname,uname) ).isEmpty;
}

#endif // ODATABASE_H
