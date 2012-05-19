#ifndef OSERVERDATABASEITEM_H
#define OSERVERDATABASEITEM_H

#include <QSqlQuery>
#include <QVariant>

//该文件内容由 makeSqlHeader.php 脚本自动生成
//用于为OServerDataBase,提供数据库结构的相关信息

namespace OSDB
{

class User
{
public:
    User():_isEmpty(true)
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
        _isEmpty(true){}

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

    static QString _table()
    {
        return "user";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(QString("uid"),QString::number(uid)));
        _result_.append(qMakePair(QString("uname"),uname));
        _result_.append(qMakePair(QString("pwd"),pwd));
        _result_.append(qMakePair(QString("lastLoginTime"),QString::number(lastLoginTime)));
        _result_.append(qMakePair(QString("lastLoginIp"),lastLoginIp));
        _result_.append(qMakePair(QString("regTime"),QString::number(regTime)));
        _result_.append(qMakePair(QString("onlineTime"),QString::number(onlineTime)));
        _result_.append(qMakePair(QString("website"),website));
        _result_.append(qMakePair(QString("info"),info));
        _result_.append(qMakePair(QString("email"),email));
        _result_.append(qMakePair(QString("avatar"),avatar));
        return _result_;
    }

    bool _isEmpty;
};

class Group
{
public:
    Group():_isEmpty(true)
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
        _isEmpty(true){}

    int gid;
    QString groupname;
    QString caption;
    QString master;
    unsigned int regTime;
    QString website;
    QString info;
    QString avatar;

    static QString _table()
    {
        return "group";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(QString("gid"),QString::number(gid)));
        _result_.append(qMakePair(QString("groupname"),groupname));
        _result_.append(qMakePair(QString("caption"),caption));
        _result_.append(qMakePair(QString("master"),master));
        _result_.append(qMakePair(QString("regTime"),QString::number(regTime)));
        _result_.append(qMakePair(QString("website"),website));
        _result_.append(qMakePair(QString("info"),info));
        _result_.append(qMakePair(QString("avatar"),avatar));
        return _result_;
    }

    bool _isEmpty;
};

class UserList
{
public:
    UserList():_isEmpty(true)
    {

    }
    UserList(QSqlQuery *query):
        id(query->value(0).toInt()),
        uname(query->value(1).toString()),
        user(query->value(2).toString()),
        _isEmpty(true){}

    int id;
    QString uname;
    QString user;

    static QString _table()
    {
        return "userlist";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(QString("id"),QString::number(id)));
        _result_.append(qMakePair(QString("uname"),uname));
        _result_.append(qMakePair(QString("user"),user));
        return _result_;
    }

    bool _isEmpty;
};

class GroupMember
{
public:
    GroupMember():_isEmpty(true)
    {

    }
    GroupMember(QSqlQuery *query):
        id(query->value(0).toInt()),
        groupname(query->value(1).toString()),
        uname(query->value(2).toString()),
        isAdmin(query->value(3).toBool()),
        isDeny(query->value(4).toBool()),
        regTime(query->value(5).toUInt()),
        _isEmpty(true){}

    int id;
    QString groupname;
    QString uname;
    bool isAdmin;
    bool isDeny;
    unsigned int regTime;

    static QString _table()
    {
        return "group_member";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(QString("id"),QString::number(id)));
        _result_.append(qMakePair(QString("groupname"),groupname));
        _result_.append(qMakePair(QString("uname"),uname));
        _result_.append(qMakePair(QString("isAdmin"),QString::number(isAdmin)));
        _result_.append(qMakePair(QString("isDeny"),QString::number(isDeny)));
        _result_.append(qMakePair(QString("regTime"),QString::number(regTime)));
        return _result_;
    }

    bool _isEmpty;
};

class UserRequest
{
public:
    UserRequest():_isEmpty(true)
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
        _isEmpty(true){}

    int id;
    unsigned int time;
    QString uname;
    QString user;
    QString invitation;
    QString msg;
    bool isHandle;
    unsigned int handleTime;
    bool result;

    static QString _table()
    {
        return "userrequest";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(QString("id"),QString::number(id)));
        _result_.append(qMakePair(QString("time"),QString::number(time)));
        _result_.append(qMakePair(QString("uname"),uname));
        _result_.append(qMakePair(QString("user"),user));
        _result_.append(qMakePair(QString("invitation"),invitation));
        _result_.append(qMakePair(QString("msg"),msg));
        _result_.append(qMakePair(QString("isHandle"),QString::number(isHandle)));
        _result_.append(qMakePair(QString("handleTime"),QString::number(handleTime)));
        _result_.append(qMakePair(QString("result"),QString::number(result)));
        return _result_;
    }

    bool _isEmpty;
};

class MsgLog
{
public:
    MsgLog():_isEmpty(true)
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
        _isEmpty(true){}

    int id;
    unsigned int time;
    unsigned int signTime;
    QString uname;
    QString user;
    QString msg;
    bool isSign;

    static QString _table()
    {
        return "msg_log";
    }

    QVector<QPair<QString,QString> > _values()
    {
        QVector<QPair<QString,QString> > _result_;
        _result_.append(qMakePair(QString("id"),QString::number(id)));
        _result_.append(qMakePair(QString("time"),QString::number(time)));
        _result_.append(qMakePair(QString("signTime"),QString::number(signTime)));
        _result_.append(qMakePair(QString("uname"),uname));
        _result_.append(qMakePair(QString("user"),user));
        _result_.append(qMakePair(QString("msg"),msg));
        _result_.append(qMakePair(QString("isSign"),QString::number(isSign)));
        return _result_;
    }

    bool _isEmpty;
};

}       //namespace OSDB

#endif // OSERVERDATABASEITEM_H
