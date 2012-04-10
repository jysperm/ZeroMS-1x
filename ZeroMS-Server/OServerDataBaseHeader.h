#ifndef OSERVERDATABASEITEM_H
#define OSERVERDATABASEITEM_H

#include <QSqlQuery>
#include <QVariant>
#include <QVector>

namespace OSDB
{

class User
{
public:
    User()
    {

    }
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
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(_uid,QString::number(uid)));
        _result_.append(qMakePair(_uname,uname));
        _result_.append(qMakePair(_pwd,pwd));
        _result_.append(qMakePair(_lastLoginTime,QString::number(lastLoginTime)));
        _result_.append(qMakePair(_lastLoginIp,lastLoginIp));
        _result_.append(qMakePair(_regTime,QString::number(regTime)));
        _result_.append(qMakePair(_onlineTime,QString::number(onlineTime)));
        _result_.append(qMakePair(_website,website));
        _result_.append(qMakePair(_info,info));
        _result_.append(qMakePair(_email,email));
        _result_.append(qMakePair(_avatar,avatar));
        return _result_;
    }

    bool _isEmpty;
};

class Group
{
public:
    Group()
    {

    }
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
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(_gid,QString::number(gid)));
        _result_.append(qMakePair(_groupname,groupname));
        _result_.append(qMakePair(_caption,caption));
        _result_.append(qMakePair(_master,master));
        _result_.append(qMakePair(_regTime,QString::number(regTime)));
        _result_.append(qMakePair(_website,website));
        _result_.append(qMakePair(_info,info));
        _result_.append(qMakePair(_avatar,avatar));
        return _result_;
    }

    bool _isEmpty;
};

class UserList
{
public:
    UserList()
    {

    }
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
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(_id,QString::number(id)));
        _result_.append(qMakePair(_uname,uname));
        _result_.append(qMakePair(_user,user));
        return _result_;
    }

    bool _isEmpty;
};

class GroupMember
{
public:
    GroupMember()
    {

    }
    GroupMember(QSqlQuery *query):
        id(query->value(0).toInt()),
        groupname(query->value(1).toString()),
        uname(query->value(2).toString()),
        isAdmin(query->value(3).toBool()),
        isDeny(query->value(4).toBool()),
        regTime(query->value(5).toUInt()),
        _isEmpty(false){}

    int id;
    QString groupname;
    QString uname;
    bool isAdmin;
    bool isDeny;
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
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(_id,QString::number(id)));
        _result_.append(qMakePair(_groupname,groupname));
        _result_.append(qMakePair(_uname,uname));
        _result_.append(qMakePair(_isAdmin,QString::number(isAdmin)));
        _result_.append(qMakePair(_isDeny,QString::number(isDeny)));
        _result_.append(qMakePair(_regTime,QString::number(regTime)));
        return _result_;
    }

    bool _isEmpty;
};

class UserRequest
{
public:
    UserRequest()
    {

    }
    UserRequest(QSqlQuery *query):
        id(query->value(0).toInt()),
        time(query->value(1).toUInt()),
        uname(query->value(2).toString()),
        user(query->value(3).toString()),
        invitation(query->value(4).toString()),
        msg(query->value(5).toString()),
        isHandle(query->value(6).toBool()),
        handleTime(query->value(7).toUInt()),
        result(query->value(8).toBool()),
        _isEmpty(false){}

    int id;
    unsigned int time;
    QString uname;
    QString user;
    QString invitation;
    QString msg;
    bool isHandle;
    unsigned int handleTime;
    bool result;

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
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(_id,QString::number(id)));
        _result_.append(qMakePair(_time,QString::number(time)));
        _result_.append(qMakePair(_uname,uname));
        _result_.append(qMakePair(_user,user));
        _result_.append(qMakePair(_invitation,invitation));
        _result_.append(qMakePair(_msg,msg));
        _result_.append(qMakePair(_isHandle,QString::number(isHandle)));
        _result_.append(qMakePair(_handleTime,QString::number(handleTime)));
        _result_.append(qMakePair(_result,QString::number(result)));
        return _result_;
    }

    bool _isEmpty;
};

class MsgLog
{
public:
    MsgLog()
    {

    }
    MsgLog(QSqlQuery *query):
        id(query->value(0).toInt()),
        time(query->value(1).toUInt()),
        signTime(query->value(2).toUInt()),
        uname(query->value(3).toString()),
        user(query->value(4).toString()),
        msg(query->value(5).toString()),
        isSign(query->value(6).toBool()),
        _isEmpty(false){}

    int id;
    unsigned int time;
    unsigned int signTime;
    QString uname;
    QString user;
    QString msg;
    bool isSign;

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
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(_id,QString::number(id)));
        _result_.append(qMakePair(_time,QString::number(time)));
        _result_.append(qMakePair(_signTime,QString::number(signTime)));
        _result_.append(qMakePair(_uname,uname));
        _result_.append(qMakePair(_user,user));
        _result_.append(qMakePair(_msg,msg));
        _result_.append(qMakePair(_isSign,QString::number(isSign)));
        return _result_;
    }

    bool _isEmpty;
};

}       //namespace OSDB

#endif // OSERVERDATABASEITEM_H
