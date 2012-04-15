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
    OT(QString k,QVariant v);

    inline OSDB::Querys operator && (OSDB::OT t);
    inline OSDB::Querys operator || (OSDB::OT t);

    QString k;
    QVariant v;
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
    //资源类
    //该类封装数据库相关操作，运行时仅存在一个实例
public:
    OServerDataBase();
    ~OServerDataBase();

    template<class T> T selectFrist(OSDB::Querys querys=OSDB::Querys(),QString order=QString(),bool isASC=true);
    template<class T> QVector<T> select(OSDB::Querys querys=OSDB::Querys(),QString order=QString(),int start=-1,int num=-1,bool isASC=true);

    template<class T> int update(OSDB::Querys querys,QString field,QVariant value);
    template<class T> void update(T source,T target);
    template<class T> void update(OSDB::Querys querys,T target);

    template<class T> int insert(T value);

    template<class T> int deleteItem(OSDB::Querys querys);

    inline bool checkUser(QString uname);//检查一个用户是否存在
    inline bool checkPWD(QString uname,QString pwdHash,QString publicKey);//检查密码是否正确
    inline bool checkGroup(QString group);//检查一个小组是否存在
    inline bool checkGroupMember(QString group,QString uname);//检查uname是否在group小组中
private:
    QSqlDatabase *dbConn;
};

#include "OServerDataBase.inline.h"

#endif // ODATABASE_H
