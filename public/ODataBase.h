#ifndef PUBLIC_ODATABASE_H
#define PUBLIC_ODATABASE_H

#include <QString>
#include <QVariant>
#include <QSqlQuery>
class QSqlDatabase;
class OQuerys;

class OQuery
{
    //值类
    //该类表示一个SQL查询条件
public:
    OQuery();
    OQuery(QString k,QVariant v);

    inline OQuerys operator && (OQuery t);
    inline OQuerys operator || (OQuery t);

    QString k;
    QVariant v;
};

class OQuerys
{
    //值类
    //该类表示一组SQL查询条件
public:
    OQuerys();
    OQuerys(OQuery t);

    inline OQuerys operator && (OQuery t);
    inline OQuerys operator || (OQuery t);

    inline QVariant &checkBoolValue(QVariant &value);

    QString getSQL();

    QString sql;
};


class ODataBase
{
    //资源类
    //该类封装数据库相关操作
public:
    ODataBase(QString drive, QString host, QString name, QString user, QString password);
    ~ODataBase();

    template<class T> T selectFrist(OQuerys querys=OQuerys(),QString order=QString(),bool isASC=true);
    template<class T> T selectFrist(QString k,QVariant v);
    template<class T> QVector<T> select(OQuerys querys=OQuerys(),QString order=QString(),int start=-1,int num=-1,bool isASC=true);
    template<class T> QVector<T> select(QString k,QVariant v);
    template<class T> int update(OQuerys querys,QString field,QVariant value);
    template<class T> int update(QString k,QVariant v,QString field,QVariant value);
    template<class T> int insert(T value);
    template<class T> int deleteItem(OQuerys querys);
    template<class T> int deleteItem(QString k,QVariant v);
private:
    QSqlDatabase *dbConn;
};

//class OQuery
//public:
inline OQuerys OQuery::operator && (OQuery t)
{
    OQuerys querys(*this);
    return (querys && t);
}

inline OQuerys OQuery::operator || (OQuery t)
{
    OQuerys querys(*this);
    return (querys || t);
}

//class OQuerys
//public:
inline OQuerys OQuerys::operator && (OQuery t)
{
    checkBoolValue(t.v);

    QString expr=QString("( `%1` = '%2' )").arg(t.k).arg(t.v.toString().replace("'","\\'"));

    if(sql.isEmpty())
        sql=expr;
    else
        sql=QString("( %1 AND %2 )").arg(sql).arg(expr);

    return *this;
}

inline OQuerys OQuerys::operator || (OQuery t)
{
    checkBoolValue(t.v);

    QString expr=QString("( `%1` = '%2' )").arg(t.k).arg(t.v.toString().replace("'","\\'"));

    if(sql.isEmpty())
        sql=expr;
    else
        sql=QString("( %1 OR %2 )").arg(sql).arg(expr);

    return *this;
}

inline QVariant &OQuerys::checkBoolValue(QVariant &value)
{
    if(QString(value.typeName())==QString("bool"))
        value=int(value.toBool());
    return value;
}

//class OServerDataBase
//public:
template<class T> T ODataBase::selectFrist(OQuerys querys,QString order,bool isASC)
{
    QSqlQuery query(*dbConn);
    QString sql=QString("SELECT * FROM `%1`").arg(T::_table());

    sql.append(querys.getSQL());

    if(!order.isEmpty())
        sql.append(" ORDER BY `%1` ").arg(order);
    if(!isASC)
        sql.append("DESC");

    sql.append(" LIMIT 0,1 ");

    query.exec(sql);

    T result;

    if(query.next())
    {
        result=&query;
        result._isEmpty=false;
    }

    return result;
}

template<class T> T ODataBase::selectFrist(QString k,QVariant v)
{
    return selectFrist<T>(OQuery(k,v));
}

template<class T> QVector<T> ODataBase::select(OQuerys querys,QString order,int start,int num,bool isASC)
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

template<class T> QVector<T> ODataBase::select(QString k,QVariant v)
{
    return select<T>(OQuery(k,v));
}

template<class T> int ODataBase::update(OQuerys querys,QString field,QVariant value)
{
    if(QString(value.typeName())==QString("bool"))
    {
        if(value.toBool()==true)
            value=1;
        else
            value=0;
    }

    QSqlQuery query(*dbConn);
    QString sql=QString("UPDATE `%1` SET `%2` = '%3'").arg(T::_table()).arg(field).arg(value.toString().replace("'","\\'"));

    sql.append(querys.getSQL());

    query.exec(sql);

    return query.numRowsAffected();
}

template<class T> int ODataBase::update(QString k,QVariant v,QString field,QVariant value)
{
    return update<T>(OQuery(k,v),field,value);
}

template<class T> int ODataBase::insert(T value)
{
    QSqlQuery query(*dbConn);

    QVector<QPair<QString,QString> > values=value._values();

    QString sql=QString("INSERT INTO `%1` VALUES (").arg(T::_table());

    QVectorIterator<QPair<QString,QString> > i(values);

    //如果第一个字段是id，设置为NULL并跳过这个字段
    if(i.peekNext().first.indexOf("id")>-1)
    {
        sql.append("NULL,");
        i.next();
    }

    while(i.hasNext())
    {
        QPair<QString,QString> value=i.next();

        sql.append(QString("'%1'").arg(value.second.replace("'","\\'")));

        if(i.hasNext())
            sql.append(",");
    }

    sql.append(")");

    query.exec(sql);

    return query.lastInsertId().toInt();
}

template<class T> int ODataBase::deleteItem(OQuerys querys)
{
    QSqlQuery query(*dbConn);
    QString sql=QString("DELETE FROM `%1`").arg(T::_table());

    sql.append(querys.getSQL());

    query.exec(sql);

    return query.numRowsAffected();
}

template<class T> int ODataBase::deleteItem(QString k,QVariant v)
{
    return deleteItem<T>(OQuery(k,v));
}

#endif // PUBLIC_ODATABASE_H
