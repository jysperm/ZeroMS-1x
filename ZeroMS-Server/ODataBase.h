#ifndef ODATABASE_H
#define ODATABASE_H

#include <QDebug>
#include "OClient.h"
class QSqlDatabase;

class ODataBase
{
public:
    class GroupInfo
    {
    public:
        int gid;
        QString groupname;
        QString caption;
        QString master;
        unsigned int regtime;
        QString website;
        QString info;
        QString avatar;
        bool isTemp;
    };

    class UserInfo
    {
    public:
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
    };

    class UserGroupStatus
    {
    public:
        int id;
        QString group;
        QString uname;
        bool isAdmin;
        bool isDeny;
        unsigned int regtime;
    };

    ODataBase();
    bool checkPWD(QString uname,QString pwd,QString publicKey);
    bool checkUser(QString uname);
    bool checkGroup(QString group);
    void removeGroupMember(QString group,QString uname);
    void ModifyUserList(QString uname,QString user,bool isAddOrRemove);
    QVector<QString> getAllGroup(QString uname=QString());
    QVector<QString> getGroupMembers(QString group);
    GroupInfo getGroupInfo(QString group);
    UserInfo getUserInfo(QString uname);
    UserGroupStatus getGroupStatus(QString uname,QString group);
private:
    QSqlDatabase *dbConn;
};

#endif // ODATABASE_H
