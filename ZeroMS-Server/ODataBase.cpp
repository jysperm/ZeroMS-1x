#include <QSqlDatabase>
#include <QCoreApplication>
#include <QStringList>
#include <QSqlError>
#include "ODataBase.h"
#include "OServerCore.h"

ODataBase::ODataBase()
{
    //调试信息
    qDebug()<<QSqlDatabase::drivers();

    //连接数据库
    dbConn=new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
    dbConn->setHostName((*config)["MYSQL_SERVER"].toString());
    dbConn->setDatabaseName((*config)["MYSQL_DATABASE"].toString());
    dbConn->setUserName((*config)["MYSQL_USER"].toString());
    dbConn->setPassword((*config)["MYSQL_PWD"].toString());

    if(!dbConn->open())
    {
        qDebug()<<QObject::tr("数据库连接失败 %1").arg(dbConn->lastError().text());
        qApp->quit();
    }
}
