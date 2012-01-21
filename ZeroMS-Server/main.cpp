#include <QCoreApplication>
#include <QTextCodec>
#include "OServerCore.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    OServerCore server;
    server.init();
    server.start();

    return a.exec();
}
