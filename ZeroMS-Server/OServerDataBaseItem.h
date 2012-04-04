#ifndef OSERVERDATABASEITEM_H
#define OSERVERDATABASEITEM_H

#include <QSqlQuery>
#include <QVariant>

namespace OSDB
{

class User
{
public:
    User();
    User(QSqlQuery *query):
        uid(query->value(0).toInt()),
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
    Group();
    Group(QSqlQuery *query):
        gid(query->value(0).toInt()),
        groupname(query->value(1).toString()),
        caption(query->value(2).toString()),
        master(query->value(3).toString()),
        regtime(query->value(4).toUInt()),
        info(query->value(5).toString()),
        website(query->value(6).toString()),
        avatar(query->value(7).toString()),
        isEmpty(false){}

    int gid;
    QString groupname;
    QString caption;
    QString master;
    unsigned int regtime;
    QString info;
    QString website;
    QString avatar;

    QString table()
    {
        return "group";
    }

    bool isEmpty;
};

class UserList
{
public:
    UserList();
    UserList(QSqlQuery *query):
        id(query->value(0).toInt()),
        uname(query->value(1).toString()),
        user(query->value(2).toString()),
        isEmpty(false){}

    int id;
    QString uname;
    QString user;

    QString table()
    {
        return "userlist";
    }

    bool isEmpty;
};


class GroupMember
{
public:
    GroupMember();
    GroupMember(QSqlQuery *query):
        id(query->value(0).toInt()),
        groupname(query->value(1).toString()),
        uname(query->value(2).toString()),
        isAdmin(query->value(3).toBool()),
        isDeny(query->value(4).toBool()),
        regtime(query->value(5).toUInt()),
        isEmpty(false){}

    int id;
    QString groupname;
    QString uname;
    bool isAdmin;
    bool isDeny;
    unsigned int regtime;

    QString table()
    {
        return "group_member";
    }

    bool isEmpty;
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

}       //namespace OSDB

#endif // OSERVERDATABASEITEM_H
