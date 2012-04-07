#include <QDebug>
#include <QStringList>
#include <QSqlError>
#include <QCoreApplication>
#include "OServerDataBase.h"
#include "../public/OSettings.h"

OSDB::OT::OT()
{

}

OSDB::OT::OT(QString k,QString v):k(k),v(v)
{

}

OSDB::Querys::Querys()
{

}

OSDB::Querys::Querys(OT t)
{
    sql=QString("( `%1` = '%2' )").arg(t.k).arg(t.v.replace("'","\\'"));
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
