#include <QSqlDatabase>
#include <QCoreApplication>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include "ODataBase.h"
#include "OServerCore.h"

const QString CHECK_USER = "SELECT `pwd` FROM `user` WHERE `uname` = :uname";
const QString CHECK_GROUP = "SELECT `groupname` FROM `group` WHERE `groupname` = :groupname";
const QString CHECK_GROUP_MEMBER = "SELECT `groupname` FROM `group_member` WHERE `groupname` = :groupname AND `uname` = :uname";
const QString CHECK_USERLIST = "SELECT `id` FROM `userlist` WHERE `uname` = :uname AND `user` = :user";
const QString ALL_GROUP = "SELECT `groupname` FROM `group`";
const QString GET_GROUP_MEMBER = "SELECT `uname` FROM `group_member` WHERE `groupname` = :groupname";
const QString GET_USERLIST = "SELECT `id`,`uname`,`user` FROM `userlist` WHERE `uname` = :uname";
const QString GET_USERLIST_BY_USER = "SELECT `id`,`uname`,`user` FROM `userlist` WHERE `uname` = :uname AND `user` = :user";
const QString QUERY_GROUP_MEMBER_BY_UNAME = "SELECT `groupname` FROM `group_member` WHERE `uname` = :uname";
const QString GET_GROUP_STATUS = "SELECT `id`,`group`,`uname`,`isadmin`,`isdeny`,`regtime` FROM `group_member` WHERE `uname` = :uname";
const QString GET_USER_INFO = "SELECT `uid`,`uname`,`pwd`,`lastlogintime`,`lastloginip`,`regtime`,`onlinetime`,`website`,`info`,`email`,`avatar` FROM `user` WHERE `uname` = :uname";
const QString GET_GROUP_INFO = "SELECT `gid`,`groupname`,`caption`,`master`,`regtime`,`website`,`info`,`avatar`,`istemp` FROM `group` WHERE `groupname` = :groupname";
const QString USERLIST_ADD = "INSERT INTO `userlist` (`uname`,`user`) VALUES (:uname,:user)";
const QString GROUP_MEMBER_REMOVE = "DELETE FROM `group_member` WHERE `groupname` = :groupname AND `uname` = :uname";
const QString USERLIST_REMOVE = "DELETE FROM `userlist` WHERE `uname` = :uname AND `user` = :user";

ODataBase::ODataBase()
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

bool ODataBase::checkPWD(QString uname,QString pwd,QString publicKey)
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

bool ODataBase::checkUser(QString uname)
{
    QSqlQuery query(*dbConn);
    query.prepare(CHECK_USER);
    query.bindValue(":uname",uname);
    query.exec();

    return query.next();
}

bool ODataBase::checkGroup(QString group)
{
    QSqlQuery query(*dbConn);
    query.prepare(CHECK_GROUP);
    query.bindValue(":groupname",group);
    query.exec();

    return query.next();
}

bool ODataBase::checkGroupMember(QString group,QString uname)
{
    QSqlQuery query(*dbConn);
    query.prepare(CHECK_GROUP_MEMBER);
    query.bindValue(":groupname",group);
    query.bindValue(":uname",uname);
    query.exec();

    return query.next();
}

void ODataBase::removeGroupMember(QString group,QString uname)
{
    QSqlQuery query(*dbConn);
    query.prepare(GROUP_MEMBER_REMOVE);
    query.bindValue(":groupname",group);
    query.bindValue(":uname",uname);
    query.exec();

    return;
}

QVector<QString> ODataBase::getAllGroup(QString uname)
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

QVector<QString> ODataBase::getGroupMembers(QString group)
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

ODataBase::UserInfo ODataBase::getUserInfo(QString uname)
{
    QSqlQuery query(*dbConn);
    query.prepare(GET_USER_INFO);
    query.bindValue(":uname",uname);
    query.exec();

    UserInfo info;
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

ODataBase::UserGroupStatus ODataBase::getGroupStatus(QString uname,QString group)
{
    QSqlQuery query(*dbConn);
    query.prepare(GET_GROUP_STATUS);
    query.bindValue(":groupname",group);
    query.bindValue(":uname",uname);
    query.exec();

    UserGroupStatus info;
    info.id=query.value(0).toInt();
    info.group=query.value(1).toString();
    info.uname=query.value(2).toString();
    info.isAdmin=query.value(3).toBool();
    info.isDeny=query.value(4).toBool();
    info.regtime=query.value(5).toUInt();

    return info;
}

QVector<ODataBase::UserListItem> ODataBase::getUserList(QString uname,QString user)
{
    QSqlQuery query(*dbConn);
    if(user.isEmpty())
    {
        query.prepare(GET_USERLIST);
        query.bindValue(":uname",uname);
    }
    else
    {
        query.prepare(GET_USERLIST_BY_USER);
        query.bindValue(":user",user);
    }

    query.exec();

    QVector<UserListItem> result;
    while(query.next())
    {
        UserListItem item;
        item.id=query.value(0).toInt();
        item.uname=query.value(1).toString();
        item.user=query.value(2).toString();
        result.append(item);
    }

    return result;
}

ODataBase::GroupInfo ODataBase::getGroupInfo(QString group)
{
    QSqlQuery query(*dbConn);
    query.prepare(GET_GROUP_INFO);
    query.bindValue(":groupname",group);
    query.exec();

    GroupInfo info;
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

void ODataBase::ModifyUserList(QString uname,QString user,bool isAddOrRemove)
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
