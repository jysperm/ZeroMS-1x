#ifndef ODATABASE_H
#define ODATABASE_H

#include <QDebug>
class QSqlDatabase;

class ODataBase
{
public:
    ODataBase();
    bool checkPWD(QString uname,QString pwd,QString publicKey);
    void ModifyUserList(QString uname,QString user,bool isAddOrRemove);
private:
    QSqlDatabase *dbConn;
};

#endif // ODATABASE_H
