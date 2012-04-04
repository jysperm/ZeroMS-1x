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

template<class T> QVector<T> OServerDataBase::selectTable(T table,QPair<QString,QString> query,QString order="",int start=-1,int num=-1,bool isASC=true)
{
    QVector vQuery;
    vQuery.append(query);

    return selectTable(table,vQuery,order,start,num,isASC);
}

