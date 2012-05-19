#include <QDebug>
#include <QStringList>
#include <QSqlError>
#include <QCoreApplication>
#include "ODataBase.h"
#include "OSettings.h"

//class OQuery
//public::
OQuery::OQuery()
{

}

OQuery::OQuery(QString k,QVariant v):k(k),v(v)
{

}

//class OQuerys
//public::
OQuerys::OQuerys()
{

}

OQuerys::OQuerys(OQuery t)
{
    sql=QString("( `%1` = '%2' )").arg(t.k).arg(t.v.toString().replace("'","\\'"));
}

QString OQuerys::getSQL()
{
    if(!sql.isEmpty())
        sql.insert(0," WHERE ");

    return sql;
}

ODataBase::ODataBase(QString drive,QString host,QString name,QString user,QString password)
{
    //调试信息
    qDebug()<<Q_FUNC_INFO<<QSqlDatabase::drivers();

    //连接数据库
    dbConn=new QSqlDatabase(QSqlDatabase::addDatabase(drive));
    dbConn->setHostName(host);
    dbConn->setDatabaseName(name);
    dbConn->setUserName(user);
    dbConn->setPassword(password);

    if(!dbConn->open())
    {
        qDebug()<<QObject::tr("数据库连接失败 %1").arg(dbConn->lastError().text());
        Q_ASSERT(false);
    }
}

ODataBase::~ODataBase()
{
    delete dbConn;
}
