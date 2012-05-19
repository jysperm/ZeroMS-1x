#include <QApplication>
#include <QTextCodec>
#include "LoginWidget.h"
#include "../public/OSettings.h"

OSettings *config;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    config=new OSettings;

    LoginWidget w;
    w.show();
    
    return a.exec();
}
