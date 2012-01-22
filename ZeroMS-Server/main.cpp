#include <QCoreApplication>
#include <QTextCodec>
#include "OServerCore.h"

OServerCore *servercore;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    servercore=new OServerCore;
    servercore->init();
    servercore->start();

    return a.exec();
}
