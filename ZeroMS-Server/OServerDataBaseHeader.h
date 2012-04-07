#ifndef OSERVERDATABASEITEM_H
#define OSERVERDATABASEITEM_H

#include <QSqlQuery>
#include <QVariant>
#include <QPair>
#include <QVector>

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
        lastLoginIp(query->value(4).toString()),
        regTime(query->value(5).toUInt()),
        onlineTime(query->value(6).toInt()),
        website(query->value(7).toString()),
        info(query->value(8).toString()),
        email(query->value(9).toString()),
        avatar(query->value(10).toString()),
        _isEmpty(false){}

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

    static QString _table()
    {
        return "user";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > result;
        result.append(qMakePair(_uid,QString::number(uid)));
        result.append(qMakePair(_uname,uname));
        result.append(qMakePair(_pwd,pwd));
        result.append(qMakePair(_lastLoginTime,QString::number(lastLoginTime)));
        result.append(qMakePair(_lastLoginIp,lastLoginIp));
        result.append(qMakePair(_regTime,QString::number(regTime)));
        result.append(qMakePair(_onlineTime,QString::number(onlineTime)));
        result.append(qMakePair(_website,website));
        result.append(qMakePair(_info,info));
        result.append(qMakePair(_email,email));
        result.append(qMakePair(_avatar,avatar));
        return result;
    }

    bool _isEmpty;
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
        website(query->value(5).toString()),
        info(query->value(6).toString()),
        avatar(query->value(7).toString()),
        _isEmpty(false){}

    int gid;
    QString groupname;
    QString caption;
    QString master;
    unsigned int regTime;
    QString website;
    QString info;
    QString avatar;

    const static QString _gid;
    const static QString _groupname;
    const static QString _caption;
    const static QString _master;
    const static QString _regTime;
    const static QString _website;
    const static QString _info;
    const static QString _avatar;

    static QString _table()
    {
        return "group";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > result;
        result.append(qMakePair(_gid,QString::number(gid)));
        result.append(qMakePair(_groupname,groupname));
        result.append(qMakePair(_caption,caption));
        result.append(qMakePair(_master,master));
        result.append(qMakePair(_regTime,QString::number(regTime)));
        result.append(qMakePair(_website,website));
        result.append(qMakePair(_info,info));
        result.append(qMakePair(_avatar,avatar));
        return result;
    }

    bool _isEmpty;
};

const QString User::_gid="gid";
const QString User::_groupname="groupname";
const QString User::_caption="caption";
const QString User::_master="master";
const QString User::_regTime="regtime";
const QString User::_website="website";
const QString User::_info="info";
const QString User::_avatar="avatar";
class UserList
{
public:
    UserList();
    UserList(QSqlQuery *query):
        id(query->value(0).toInt()),
        uname(query->value(1).toString()),
        user(query->value(2).toString()),
        _isEmpty(false){}

    int id;
    QString uname;
    QString user;

    const static QString _id;
    const static QString _uname;
    const static QString _user;

    static QString _table()
    {
        return "userlist";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > result;
        result.append(qMakePair(_id,QString::number(id)));
        result.append(qMakePair(_uname,uname));
        result.append(qMakePair(_User,user));
        return result;
    }

    bool _isEmpty;
};

const QString User::_id="id";
const QString User::_uname="uname";
const QString User::_user="user";
class GroupMember
{
public:
    GroupMember();
    GroupMember(QSqlQuery *query):
        id(query->value(0).toInt()),
        groupname(query->value(1).toString()),
        uname(query->value(2).toString()),
        isAdmin(query->value(3).toInt()),
        isDeny(query->value(4).toInt()),
        regTime(query->value(5).toUInt()),
        _isEmpty(false){}

    int id;
    QString groupname;
    QString uname;
    int isAdmin;
    int isDeny;
    unsigned int regTime;

    const static QString _id;
    const static QString _groupname;
    const static QString _uname;
    const static QString _isAdmin;
    const static QString _isDeny;
    const static QString _regTime;

    static QString _table()
    {
        return "group_member";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > result;
        result.append(qMakePair(_id,QString::number(id)));
        result.append(qMakePair(_groupname,groupname));
        result.append(qMakePair(_uname,uname));
        result.append(qMakePair(_isAdmin,QString::number(isAdmin)));
        result.append(qMakePair(_isDeny,QString::number(isDeny)));
        result.append(qMakePair(_regTime,QString::number(regTime)));
        return result;
    }

    bool _isEmpty;
};

const QString User::_id="id";
const QString User::_groupname="groupname";
const QString User::_uname="uname";
const QString User::_isAdmin="isadmin";
const QString User::_isDeny="isdeny";
const QString User::_regTime="regtime";
class Userrequest
{
public:
    Userrequest();
    Userrequest(QSqlQuery *query):
        id(query->value(0).toInt()),
        time(query->value(1).toUInt()),
        uname(query->value(2).toString()),
        user(query->value(3).toString()),
        invitation(query->value(4).toString()),
        msg(query->value(5).toString()),
        isHandle(query->value(6).toInt()),
        handleTime(query->value(7).toUInt()),
        result(query->value(8).toInt()),
        _isEmpty(false){}

    int id;
    unsigned int time;
    QString uname;
    QString user;
    QString invitation;
    QString msg;
    int isHandle;
    unsigned int handleTime;
    int result;

    const static QString _id;
    const static QString _time;
    const static QString _uname;
    const static QString _user;
    const static QString _invitation;
    const static QString _msg;
    const static QString _isHandle;
    const static QString _handleTime;
    const static QString _result;

    static QString _table()
    {
        return "userrequest";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > result;
        result.append(qMakePair(_id,QString::number(id)));
        result.append(qMakePair(_time,QString::number(time)));
        result.append(qMakePair(_uname,uname));
        result.append(qMakePair(_User,user));
        result.append(qMakePair(_invitation,invitation));
        result.append(qMakePair(_msg,msg));
        result.append(qMakePair(_isHandle,QString::number(isHandle)));
        result.append(qMakePair(_handleTime,QString::number(handleTime)));
        result.append(qMakePair(_result,QString::number(result)));
        return result;
    }

    bool _isEmpty;
};

const QString User::_id="id";
const QString User::_time="time";
const QString User::_uname="uname";
const QString User::_user="user";
const QString User::_invitation="invitation";
const QString User::_msg="msg";
const QString User::_isHandle="ishandle";
const QString User::_handleTime="handletime";
const QString User::_result="result";
class msg_log
{
public:
    msg_log();
    msg_log(QSqlQuery *query):
        id(query->value(0).toInt()),
        time(query->value(1).toUInt()),
        signTime(query->value(2).toUInt()),
        uname(query->value(3).toString()),
        user(query->value(4).toString()),
        msg(query->value(5).toString()),
        isSign(query->value(6).toInt()),
        _isEmpty(false){}

    int id;
    unsigned int time;
    unsigned int signTime;
    QString uname;
    QString user;
    QString msg;
    int isSign;

    const static QString _id;
    const static QString _time;
    const static QString _signTime;
    const static QString _uname;
    const static QString _user;
    const static QString _msg;
    const static QString _isSign;

    static QString _table()
    {
        return "msg_log";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > result;
        result.append(qMakePair(_id,QString::number(id)));
        result.append(qMakePair(_time,QString::number(time)));
        result.append(qMakePair(_signTime,QString::number(signTime)));
        result.append(qMakePair(_uname,uname));
        result.append(qMakePair(_User,user));
        result.append(qMakePair(_msg,msg));
        result.append(qMakePair(_isSign,QString::number(isSign)));
        return result;
    }

    bool _isEmpty;
};

const QString User::_id="id";
const QString User::_time="time";
const QString User::_signTime="signtime";
const QString User::_uname="uname";
const QString User::_user="user";
const QString User::_msg="msg";
const QString User::_isSign="issign";

}       //namespace OSDB

#endif // OSERVERDATABASEITEM_H
