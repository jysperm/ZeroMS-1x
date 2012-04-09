#ifndef ODATABASE_H
#define ODATABASE_H

#include "OServerDataBaseHeader.h"
#include "global.h"
class QSqlDatabase;

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

    template<class T> T selectFrist(OSDB::Querys querys=OSDB::Querys(),QString order=QString(),bool isASC=true);
    template<class T> QVector<T> select(OSDB::Querys querys=OSDB::Querys(),QString order=QString(),int start=-1,int num=-1,bool isASC=true);

    template<class T> void update(T source,T target);
    template<class T> void update(OSDB::Querys querys,T target);
    template<class T> int update(OSDB::Querys querys,QString field,QVariant value);

    template<class T> void insert(T value);

    template<class T> int deleteItem(OSDB::Querys querys);

    inline bool checkUser(QString uname);//检查一个用户是否存在
    inline bool checkPWD(QString uname,QString pwd,QString publicKey);//检查密码是否正确
    inline bool checkGroup(QString group);//检查一个小组是否存在
    inline bool checkGroupMember(QString group,QString uname);//检查uname是否在group小组中
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
    QString expr=QString("( `%1` = '%2' )").arg(t.k).arg(t.v.replace("'","\\'"));

    if(sql.isEmpty())
        sql=expr;
    else
        sql=QString("( %1 AND %2 )").arg(sql).arg(expr);

    return *this;
}

inline OSDB::Querys OSDB::Querys::operator || (OSDB::OT t)
{
    QString expr=QString("( `%1` = '%2' )").arg(t.k).arg(t.v.replace("'","\\'"));

    if(sql.isEmpty())
        sql=expr;
    else
        sql=QString("( %1 OR %2 )").arg(sql).arg(expr);

    return *this;
}

inline bool OServerDataBase::checkUser(QString uname)
{
    using namespace OSDB;
    return !selectFrist<User>(OT(User::_uname,uname))._isEmpty;
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
    return !selectFrist<Group>(OT(Group::_groupname,group))._isEmpty;
}

inline bool OServerDataBase::checkGroupMember(QString group,QString uname)
{
    using namespace OSDB;
    return !selectFrist<GroupMember>( OT(GroupMember::_groupname,group) && OT(GroupMember::_uname,uname) )._isEmpty;
}

template<class T> T OServerDataBase::selectFrist(OSDB::Querys querys,QString order,bool isASC)
{
    QSqlQuery query(*dbConn);
    QString sql=QString("SELECT TOP 1 * FROM `%1`").arg(T::_table());

    sql.append(querys.getSQL());

    if(!order.isEmpty())
        sql.append(" ORDER BY %1 ").arg(order);
    if(!isASC)
        sql.append("DESC");

    query.exec(sql);

    bool hasNext=query.next();
    T result(&query);
    result._isEmpty=hasNext;

    return result;
}

template<class T> QVector<T> OServerDataBase::select(OSDB::Querys querys,QString order,int start,int num,bool isASC)
{
    QSqlQuery query(*dbConn);
    QString sql=QString("SELECT * FROM `%1`").arg(T::_table());

    sql.append(querys.getSQL());

    if(!order.isEmpty())
        sql.append(" ORDER BY %1 ").arg(order);
    if(!isASC)
        sql.append(" DESC ");

    if(num>-1 && start>-1)
    {
        sql.append(" LIMIT %1,%2").arg(start).arg(num);
    }
    if(num>-1 && !(start>-1))
    {
        sql.append(" LIMIT %1").arg(num);
    }

    query.exec(sql);

    QVector<T> result;
    while(query.next())
    {
        result.append(T(&query));
    }

    return result;
}

template<class T> int OServerDataBase::update(OSDB::Querys querys,QString field,QVariant value)
{
    QSqlQuery query(*dbConn);
    QString sql=QString("UPDATE `%1` SET `%2` = '%3'").arg(T::_table()).arg(field).arg(value.toString().replace("'","\\'"));
    sql.append(querys.getSQL());

    query.exec(sql);

    return query.numRowsAffected();
}

template<class T> void OServerDataBase::update(OSDB::Querys querys,T target)
{
    QSqlQuery query(*dbConn);

    QVector<QPair<QString,QString> > values=target._values();

    QVectorIterator<QPair<QString,QString> > i(values);
    while(i.hasNext())
    {
        QPair<QString,QString> value=i.next();

        QString sql=QString("UPDATE `%1` SET `%2` = '%3'").arg(T::_table()).arg(value.first).arg(value.second.replace("'","\\'"));
        sql.append(querys.getSQL());

        query.exec(sql);
    }
}

template<class T> void OServerDataBase::update(T source,T target)
{
    QSqlQuery query(*dbConn);

    OSDB::Querys querys;

    QVector<QPair<QString,QString> > sourceValues=source._values();
    QVectorIterator<QPair<QString,QString> > iS(sourceValues);
    while(iS.hasNext())
    {
        QPair<QString,QString> value=iS.next();
        querys=querys && OSDB::OT(value.first,value.second);
    }

    QVector<QPair<QString,QString> > values=target._values();
    QVectorIterator<QPair<QString,QString> > i(values);
    while(i.hasNext())
    {
        QPair<QString,QString> value=i.next();

        QString sql=QString("UPDATE `%1` SET `%2` = '%3'").arg(T::_table()).arg(value.first).arg(value.second.replace("'","\\'"));
        sql.append(querys.getSQL());

        query.exec(sql);
    }
}

template<class T> void OServerDataBase::insert(T value)
{
    QSqlQuery query(*dbConn);

    QVector<QPair<QString,QString> > values=value._values();

    QString sql=QString("INSERT INTO `%1` VALUES (").arg(T::_table());

    QVectorIterator<QPair<QString,QString> > i(values);

    //如果第一个字段是id，跳过这个字段
    if(i.peekNext().first.indexOf("id")>-1)
        i.next();

    while(i.hasNext())
    {
        QPair<QString,QString> value=i.next();

        sql.append(value.second.replace("'","\\'"));

        if(i.hasNext())
            sql.append(",");
    }

    sql.append(")");

    query.exec(sql);
}

template<class T> int OServerDataBase::deleteItem(OSDB::Querys querys)
{
    QSqlQuery query(*dbConn);
    QString sql=QString("DELETE FROM `%1`").arg(T::_table());

    sql.append(querys.getSQL());

    query.exec(sql);

    return query.numRowsAffected();
}

#endif // ODATABASE_H
