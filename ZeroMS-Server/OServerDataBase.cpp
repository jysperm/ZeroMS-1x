#include <QSqlDatabase>
#include <QCoreApplication>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include "OServerDataBase.h"
#include "OServerCore.h"

OSDB::OT::OT()
{

}

OSDB::OT::O(QString k,QString v):k(k),v(v)
{

}

OSDB::Querys::Querys()
{

}

OSDB::Querys::Querys(OT t)
{
    querys.append(t);
}

QString OSDB::Querys::getSQL()
{
    if(!sql.isEmpty())
        sql.insert(0," WHERE ");

    return sql;
}

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

template<class T> T OServerDataBase::selecFrist(OSDB::Querys querys,QString order,bool isASC)
{
    QSqlQuery query(*dbConn);
    QString sql=QString("SELECT TOP 1 * FROM `%1`").arg(T().table());

    sql.append(querys.getSQL());

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

template<class T> QVector<T> OServerDataBase::selectTable(OSDB::Querys querys,QString order,int start,int num,bool isASC)
{
    QSqlQuery query(*dbConn);
    QString sql=QString("SELECT * FROM `%1`").arg(T().table());

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
    QString sql=QString("UPDATE `%1` SET `%2` = '%3'").arg(T().table()).arg(field).arg(value.replace("'","\\'"));

    sql.append(querys.getSQL());

    query.exec(sql);

    return query.numRowsAffected();
}

template<class T> int OServerDataBase::update(OSDB::Querys querys,T target)
{

}
