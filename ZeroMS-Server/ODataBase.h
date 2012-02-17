#ifndef ODATABASE_H
#define ODATABASE_H

#include <QDebug>
class QSqlDatabase;

class ODataBase
{
public:
    ODataBase();
    bool checkPWD(QString uname,QString pwd,QString publicKey);
private:
    QSqlDatabase *dbConn;
};

#endif // ODATABASE_H
