#ifndef ODATABASE_H
#define ODATABASE_H

#include <QDebug>
#include "OClient.h"
#include <QSqlQuery>

namespace OServerDatabase
{

class User
{
public:
    User();
    User(QSqlQuery *query):uid(query->value(0).toInt()),
        uname(query->value(1).toString()),
        pwd(query->value(2).toString()),
        lastLoginTime(query->value(3).toUInt()),
        lastLoginIp(query->value(4).toInt()),
        regTime(query->value(5).toUInt()),
        onlineTime(query->value(6).toInt()),
        website(query->value(7).toString()),
        info(query->value(8).toString()),
        email(query->value(9).toString()),
        avatar(query->value(10).toString()),
        isEmpty(false){}

    int uid;
    QString uname;
    QString pwd;
    unsigned int lastLoginTime;
    QString lastLoginIp;
    unsigned int regTime;
    int onlineTime;
    QString website;
    QString info;
    QString email;
    QString avatar;

    QString table()
    {
        return "user";
    }

    bool isEmpty;
};

class Group
{
public:
    int gid;
    QString groupname;
    QString caption;
    QString master;
    unsigned int regtime;
    QString info;
    QString website;
    QString avatar;
};

class UserList
{
public:
    int id;
    QString uname;
    QString user;
};


class GroupMember
{
public:
    int id;
    QString groupname;
    QString uname;
    bool isAdmin;
    bool isDeny;
    unsigned int regtime;
};

class UserRequest
{
public:
    int id;
    unsigned int time;
    QString uname;
    QString user;
    QString invitation;
    QString msg;
    bool ishandle;
    unsigned int handletime;
    bool result;
};

class MsgLog
{
public:
    int id;
    unsigned int time;
    unsigned int sgintime;
    QString uname;
    QString user;
    QString msg;
    bool issgin;
};

}       //namespace OServerDatabase

class OServerDataBase
{
public:
    OServerDataBase();
    ~OServerDataBase();

    template<class T> T selectFrist(T table,QVector<QPair<QString,QString> > query,QString order="",bool isASC=true);
    template<class T> T selectFrist(T table,QPair<QString,QString> query,QString order="",bool isASC=true);
    template<class T> QVector<T> selectTable(T table,QVector<QPair<QString,QString> > query,QString order="",int start=-1,int num=-1,bool isASC=true);

    /*
    bool checkPWD(QString uname,QString pwd,QString publicKey);
    bool checkUser(QString uname);
    bool checkGroup(QString group);
    bool checkGroupMember(QString group,QString uname);
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

#endif // ODATABASE_H
