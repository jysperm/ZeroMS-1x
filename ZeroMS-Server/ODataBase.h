#ifndef ODATABASE_H
#define ODATABASE_H

#include <QDebug>
class QSqlDatabase;

class ODataBase
{
public:
    ODataBase();
private:
    QSqlDatabase *dbConn;
};

#endif // ODATABASE_H
