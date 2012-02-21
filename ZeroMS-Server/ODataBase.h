#ifndef ODATABASE_H
#define ODATABASE_H

#include <QDebug>
class QSqlDatabase;

class ODataBase
{
public:
    ODataBase();
    bool checkPWD(QString uname,QString pwd,QString publicKey);
    bool checkUser(QString uname);
    bool checkGroup(QString group);
    void removeGroupMember(QString group,QString uname);
    void ModifyUserList(QString uname,QString user,bool isAddOrRemove);
private:
    QSqlDatabase *dbConn;
};

#endif // ODATABASE_H
