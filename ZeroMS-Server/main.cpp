#include <QCoreApplication>
#include <QTextCodec>
#include "OServerCore.h"
#include "../public/OSettings.h"
#include "../public/ODataBase.h"

OServerCore *core;
OSettings *config;
ODataBase *db;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    config=new OSettings;
    db=new ODataBase("QMYSQL",(*config)["MYSQL_SERVER"].toString(),
                     (*config)["MYSQL_DATABASE"].toString(),
                     (*config)["MYSQL_USER"].toString(),
                     (*config)["MYSQL_PWD"].toString());

    core=new OServerCore;
    core->init();
    core->start();

    return a.exec();
}
