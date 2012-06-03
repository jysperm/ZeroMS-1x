#include <QApplication>
#include <QTextCodec>
#include "../public/OSettings.h"
#include "OClientCore.h"

OSettings *config;
OClientCore *core;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QCoreApplication::setOrganizationName(QString("零毫秒"));
    QCoreApplication::setOrganizationDomain("0-ms.org");
    QCoreApplication::setApplicationName("零毫秒官方客户端2.x");

    config=new OSettings;

    core=new OClientCore;
    core->init();
    
    int retrunCode=a.exec();

    delete config;
    delete core;

    return retrunCode;
}
