#ifndef ODATABASE_H
#define ODATABASE_H

#include <QDebug>
#include <QSqlQuery>
#include "OClient.h"
#include "OServerDataBaseItem.h"

class OServerDataBase
{
public:
    OServerDataBase();
    ~OServerDataBase();

    template<class T> T selectFrist(T table,QVector<QPair<QString,QString> > query,QString order="",bool isASC=true);
    template<class T> T selectFrist(T table,QPair<QString,QString> query,QString order="",bool isASC=true);
    template<class T> QVector<T> selectTable(T table,QVector<QPair<QString,QString> > query,QString order="",int start=-1,int num=-1,bool isASC=true);
    template<class T> QVector<T> selectTable(T table,QPair<QString,QString> query,QString order="",int start=-1,int num=-1,bool isASC=true);

    inline bool checkUser(QString uname);//检查一个用户是否存在
    inline bool checkPWD(QString uname,QString pwd,QString publicKey);//检查密码是否正确
    inline bool checkGroup(QString group);//检查一个小组是否存在
    inline bool checkGroupMember(QString group,QString uname);//检查uname是否在group小组中

    /*
    void removeGroupMember(QString group,QString uname);
    void ModifyUserList(QString uname,QString user,bool isAddOrRemove);
    QVector<QString> getAllGroup(QString uname=QString());
    QVector<QString> getGroupMembers(QString group);
    OGroup getGroupInfo(QString group);
    OUser getUserInfo(QString uname);
    OGroupMember getGroupStatus(QString uname,QString group);
    QVector<OUserList> getUserList(QString uname,QString user=QString());
    QVector<OUserList> getUserListByUser(QString user);*/
private:
    QSqlDatabase *dbConn;
};

inline bool OServerDataBase::checkUser(QString uname)
{
    return !selectFrist(OSDB::User(),OMakePair("uname",uname)).isEmpty;
}

inline bool OServerDataBase::checkPWD(QString uname,QString pwd,QString publicKey)
{
    if(pwd==OSha1(publicKey+selectFrist(OSDB::User(),OMakePair("uname",uname)).pwd))
        return true;
    return false;
}

inline bool OServerDataBase::checkGroup(QString group)
{
    return !selectFrist(OSDB::Group(),OMakePair("groupname",group)).isEmpty;
}

inline bool OServerDataBase::checkGroupMember(QString group,QString uname)
{
    QVector<QPair<QString,QString> > vQuery;
    vQuery.append(OMakePair("groupname",group));
    vQuery.append(OMakePair("uname",uname));
    return !selectFrist(OSDB::GroupMember(),vQuery).isEmpty;
}

#endif // ODATABASE_H
