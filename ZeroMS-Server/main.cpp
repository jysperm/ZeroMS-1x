#include <QCoreApplication>
#include <QTextCodec>
#include <QDateTime>
#include "OServerCore.h"

OServerCore *servercore;
OSettings *config;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    qsrand(QDateTime::currentDateTime().toTime_t());

    config=new OSettings;

    servercore=new OServerCore;
    servercore->init();
    servercore->start();

    return a.exec();
}
