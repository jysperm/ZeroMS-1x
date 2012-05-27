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

    config=new OSettings;

    core=new OClientCore;
    core->init();
    
    return a.exec();
}
