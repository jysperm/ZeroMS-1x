#include <QSqlDatabase>
#include <QCoreApplication>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include "ODataBase.h"
#include "OServerCore.h"

const QString CHECK_USER = "SELECT `pwd` FROM `user` WHERE `uname` = :uname";
const QString CHECK_GROUP = "SELECT `groupname` FROM `group` WHERE `groupname` = :groupname";
const QString CHECK_USERLIST = "SELECT `id` FROM `userlist` WHERE `uname` = :uname AND `user` = :user";
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

void ODataBase::removeGroupMember(QString group,QString uname)
{
    QSqlQuery query(*dbConn);
    query.prepare(GROUP_MEMBER_REMOVE);
    query.bindValue(":groupname",group);
    query.bindValue(":uname",uname);
    query.exec();

    return;
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
