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

    const static QString _uid;
    const static QString _uname;
    const static QString _pwd;
    const static QString _lastLoginTime;
    const static QString _lastLoginIp;
    const static QString _regTime;
    const static QString _onlineTime;
    const static QString _website;
    const static QString _info;
    const static QString _email;
    const static QString _avatar;

    QString table()
    {
        return "user";
    }

    bool isEmpty;
};

const QString User::_uid="uid";
const QString User::_uname="uname";
const QString User::_pwd="pwd";
const QString User::_lastLoginTime="lastlogintime";
const QString User::_lastLoginIp="lastloginip";
const QString User::_regTime="regtime";
const QString User::_onlineTime="onlinetime";
const QString User::_website="website";
const QString User::_info="info";
const QString User::_email="email";
const QString User::_avatar="avatar";

class Group
{
public:
    Group();
    Group(QSqlQuery *query):
        gid(query->value(0).toInt()),
        groupname(query->value(1).toString()),
        caption(query->value(2).toString()),
        master(query->value(3).toString()),
        regTime(query->value(4).toUInt()),
        info(query->value(5).toString()),
        website(query->value(6).toString()),
        avatar(query->value(7).toString()),
        isEmpty(false){}

    int gid;
    QString groupname;
    QString caption;
    QString master;
    unsigned int regTime;
    QString info;
    QString website;
    QString avatar;

    const static QString _gid;
    const static QString _groupname;
    const static QString _caption;
    const static QString _master;
    const static QString _regTime;
    const static QString _info;
    const static QString _website;
    const static QString _avatar;

    QString table()
    {
        return "group";
    }

    bool isEmpty;
};

const QString Group::_gid="gid";
const QString Group::_groupname="groupname";
const QString Group::_caption="caption";
const QString Group::_master="master";
const QString Group::_regTime="regtime";
const QString Group::_info="info";
const QString Group::_website="website";
const QString Group::_avatar="avatar";


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

    const static QString _id;
    const static QString _uname;
    const static QString _user;

    QString table()
    {
        return "userlist";
    }

    bool isEmpty;
};

const QString UserList::_id="id";
const QString UserList::_uname="uname";
const QString UserList::_user="user";


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

    const static QString _id;
    const static QString _groupname;
    const static QString _uname;
    const static QString _isAdmin;
    const static QString _isDeny;
    const static QString _regTime;

    QString table()
    {
        return "group_member";
    }

    bool isEmpty;
};

const QString GroupMember::_id="id";
const QString GroupMember::_groupname="groupname";
const QString GroupMember::_uname="uname";
const QString GroupMember::_isAdmin="isAdmin";
const QString GroupMember::_isDeny="isDeny";
const QString GroupMember::_regTime="regtime";

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
