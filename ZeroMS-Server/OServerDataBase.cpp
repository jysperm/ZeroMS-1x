#include <QSqlDatabase>
#include <QCoreApplication>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include "OServerDataBase.h"
#include "OServerCore.h"

const QString CHECK_USER = "SELECT `pwd` FROM `user` WHERE `uname` = :uname";
const QString CHECK_GROUP = "SELECT `groupname` FROM `group` WHERE `groupname` = :groupname";
const QString CHECK_GROUP_MEMBER = "SELECT `groupname` FROM `group_member` WHERE `groupname` = :groupname AND `uname` = :uname";
const QString CHECK_USERLIST = "SELECT `id` FROM `userlist` WHERE `uname` = :uname AND `user` = :user";
const QString ALL_GROUP = "SELECT `groupname` FROM `group`";
const QString GET_GROUP_MEMBER = "SELECT `uname` FROM `group_member` WHERE `groupname` = :groupname";
const QString GET_USERLIST = "SELECT `id`,`uname`,`user` FROM `userlist` WHERE `uname` = :uname";
const QString GET_USERLIST_BY_UNAME_USER = "SELECT `id`,`uname`,`user` FROM `userlist` WHERE `uname` = :uname AND `user` = :user";
const QString GET_USERLIST_BY_USER = "SELECT `id`,`uname`,`user` FROM `userlist` WHERE `user` = :user";
const QString QUERY_GROUP_MEMBER_BY_UNAME = "SELECT `groupname` FROM `group_member` WHERE `uname` = :uname";
const QString GET_GROUP_STATUS = "SELECT `id`,`groupname`,`uname`,`isadmin`,`isdeny`,`regtime` FROM `group_member` WHERE `groupname` = :groupname AND `uname` = :uname";
const QString GET_USER_INFO = "SELECT `uid`,`uname`,`pwd`,`lastlogintime`,`lastloginip`,`regtime`,`onlinetime`,`website`,`info`,`email`,`avatar` FROM `user` WHERE `uname` = :uname";
const QString GET_GROUP_INFO = "SELECT `gid`,`groupname`,`caption`,`master`,`regtime`,`website`,`info`,`avatar`,`istemp` FROM `group` WHERE `groupname` = :groupname";
const QString USERLIST_ADD = "INSERT INTO `userlist` (`uname`,`user`) VALUES (:uname,:user)";
const QString GROUP_MEMBER_REMOVE = "DELETE FROM `group_member` WHERE `groupname` = :groupname AND `uname` = :uname";
const QString USERLIST_REMOVE = "DELETE FROM `userlist` WHERE `uname` = :uname AND `user` = :user";

OServerDataBase::OServerDataBase()
{
    //调试信息
    qDebug()<<Q_FUNC_INFO<<QSqlDatabase::drivers();

    //连接数据库
    dbConn=new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
    dbConn->setHostName((*config)["MYSQL_SERVER"].toString());
    dbConn->setDatabaseName((*config)["MYSQL_DATABASE"].toString());
    dbConn->setUserName((*config)["MYSQL_USER"].toString());
    dbConn->setPassword((*config)["MYSQL_PWD"].toString());

    if(!dbConn->open())
    {
        qDebug()<<Q_FUNC_INFO<<QObject::tr("数据库连接失败 %1").arg(dbConn->lastError().text());
        qApp->quit();
    }
}

OServerDataBase::~OServerDataBase()
{
    delete dbConn;
}

template<class T> T OServerDataBase::selecFrist(T table,QVector<QPair<QString,QString> > query,QString order,bool isASC)
{
    QSqlQuery query(*dbConn);
    QString sql=QString("SELECT TOP 1 * FROM `%1`").arg(table.table());

    QVectorIterator i(query);
    if(i.hasNext())
        sql.append(" WHERE ");
    while(i.hasNext())
    {
        QPair<QString,QString> pair=i.next();

        sql.append(QString(" `%1` = '%2'").append(pair.second.replace("'","\\'")));

        if(i.hasNext())
            sql.append(" AND ");
    }

    if(!order.isEmpty())
        sql.append(" ORDER BY %1 ").arg(order);
    if(!isASC)
        sql.append("DESC");

    query.exec(sql);

    bool hasNext=query.next();
    T result(&query);
    result.isEmpty=hasNext;

    return result;
}

template<class T> T OServerDataBase::selectFrist(T table,QPair<QString,QString> query,QString order,bool isASC)
{
    QVector vQuery;
    vQuery.append(query);

    return selectFrist(table,vQuery,order,isASC);
}

template<class T> QVector<T> OServerDataBase::selectTable(T table,QVector<QPair<QString,QString> > query,QString order,int start,int num,bool isASC)
{
    QSqlQuery query(*dbConn);
    QString sql=QString("SELECT * FROM `%1`").arg(table.table());

    QVectorIterator i(query);
    if(i.hasNext())
        sql.append(" WHERE ");
    while(i.hasNext())
    {
        QPair<QString,QString> pair=i.next();

        sql.append(QString(" `%1` = '%2'").append(pair.second.replace("'","\\'")));

        if(i.hasNext())
            sql.append(" AND ");
    }

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



/*
bool OServerDataBase::checkPWD(QString uname,QString pwd,QString publicKey)
{
    QSqlQuery query(*dbConn);
    query.prepare(CHECK_USER);
    query.bindValue(":uname",uname);
    query.exec();

    if(query.next())
        if(pwd==Osha1(publicKey+query.value(0).toString()))
            return true;

    return false;
}

bool OServerDataBase::checkUser(QString uname)
{
    QSqlQuery query(*dbConn);
    query.prepare(CHECK_USER);
    query.bindValue(":uname",uname);
    query.exec();

    return query.next();
}

bool OServerDataBase::checkGroup(QString group)
{
    QSqlQuery query(*dbConn);
    query.prepare(CHECK_GROUP);
    query.bindValue(":groupname",group);
    query.exec();

    return query.next();
}

bool OServerDataBase::checkGroupMember(QString group,QString uname)
{
    QSqlQuery query(*dbConn);
    query.prepare(CHECK_GROUP_MEMBER);
    query.bindValue(":groupname",group);
    query.bindValue(":uname",uname);
    query.exec();

    return query.next();
}

void OServerDataBase::removeGroupMember(QString group,QString uname)
{
    QSqlQuery query(*dbConn);
    query.prepare(GROUP_MEMBER_REMOVE);
    query.bindValue(":groupname",group);
    query.bindValue(":uname",uname);
    query.exec();

    return;
}

QVector<QString> OServerDataBase::getAllGroup(QString uname)
{
    QSqlQuery query(*dbConn);
    if(uname.isEmpty())
        query.prepare(ALL_GROUP);
    else
    {
        query.prepare(QUERY_GROUP_MEMBER_BY_UNAME);
        query.bindValue(":uname",uname);
    }

    query.exec();

    QVector<QString> result;
    while(query.next())
        result.append(query.value(0).toString());

    return result;
}

QVector<QString> OServerDataBase::getGroupMembers(QString group)
{
    QSqlQuery query(*dbConn);
    query.prepare(GET_GROUP_MEMBER);
    query.bindValue(":groupname",group);

    query.exec();

    QVector<QString> result;
    while(query.next())
        result.append(query.value(0).toString());

    return result;
}

OServerDataBase::UserInfo OServerDataBase::getUserInfo(QString uname)
{
    QSqlQuery query(*dbConn);
    query.prepare(GET_USER_INFO);
    query.bindValue(":uname",uname);
    query.exec();

    query.next();

    OUser info;
    info.uid=query.value(0).toInt();
    info.uname=query.value(1).toString();
    info.pwd=query.value(2).toString();
    info.lastLoginTime=query.value(3).toUInt();
    info.lastLoginIp=query.value(4).toString();
    info.regTime=query.value(5).toUInt();
    info.onlineTime=query.value(6).toInt();
    info.website=query.value(7).toString();
    info.info=query.value(8).toString();
    info.email=query.value(9).toString();
    info.avatar=query.value(10).toString();

    return info;
}

OServerDataBase::UserGroupStatus OServerDataBase::getGroupStatus(QString uname,QString group)
{
    QSqlQuery query(*dbConn);
    query.prepare(GET_GROUP_STATUS);
    query.bindValue(":groupname",group);
    query.bindValue(":uname",uname);
    query.exec();

    query.next();

    OGroupMember info;
    info.id=query.value(0).toInt();
    info.groupname=query.value(1).toString();
    info.uname=query.value(2).toString();
    info.isAdmin=query.value(3).toBool();
    info.isDeny=query.value(4).toBool();
    info.regtime=query.value(5).toUInt();

    return info;
}

QVector<OServerDataBase::UserListItem> OServerDataBase::getUserList(QString uname,QString user)
{
    QSqlQuery query(*dbConn);
    if(user.isEmpty())
    {
        query.prepare(GET_USERLIST);
        query.bindValue(":uname",uname);
    }
    else
    {
        query.prepare(GET_USERLIST_BY_UNAME_USER);
        query.bindValue(":uname",uname);
        query.bindValue(":user",user);
    }

    query.exec();

    QVector<OUserList> result;
    while(query.next())
    {
        OUserList item;
        item.id=query.value(0).toInt();
        item.uname=query.value(1).toString();
        item.user=query.value(2).toString();
        result.append(item);
    }

    return result;
}

QVector<OServerDataBase::UserListItem> OServerDataBase::getUserListByUser(QString user)
{
    QSqlQuery query(*dbConn);

    query.prepare(GET_USERLIST_BY_USER);
    query.bindValue(":user",user);

    query.exec();

    QVector<OUserList> result;
    while(query.next())
    {
        OUserList item;
        item.id=query.value(0).toInt();
        item.uname=query.value(1).toString();
        item.user=query.value(2).toString();
        result.append(item);
    }

    return result;
}

OServerDataBase::GroupInfo OServerDataBase::getGroupInfo(QString group)
{
    QSqlQuery query(*dbConn);
    query.prepare(GET_GROUP_INFO);
    query.bindValue(":groupname",group);
    query.exec();

    query.next();

    OGroup info;
    info.gid=query.value(0).toInt();
    info.groupname=query.value(1).toString();
    info.caption=query.value(2).toString();
    info.master=query.value(3).toString();
    info.regtime=query.value(4).toUInt();
    info.website=query.value(5).toString();
    info.info=query.value(6).toString();
    info.avatar=query.value(7).toString();
    info.isTemp=query.value(8).toBool();

    return info;
}

void OServerDataBase::ModifyUserList(QString uname,QString user,bool isAddOrRemove)
{
    QSqlQuery query(*dbConn);
    query.prepare(CHECK_USERLIST);
    query.bindValue(":uname",uname);
    query.bindValue(":user",user);
    query.exec();

    if(query.next())
    {//如果存在这条记录
        if(!isAddOrRemove)
        {//如果是删除操作
            QSqlQuery query(*dbConn);
            query.prepare(USERLIST_REMOVE);
            query.bindValue(":uname",uname);
            query.bindValue(":user",user);
            query.exec();
        }
    }
    else
    {//如果没有这条记录
        if(isAddOrRemove)
        {//如果是添加操作
            QSqlQuery query(*dbConn);
            query.prepare(USERLIST_ADD);
            query.bindValue(":uname",uname);
            query.bindValue(":user",user);
            query.exec();
        }
    }
}
*/
