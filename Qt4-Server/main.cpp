#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QTextCodec>
#include "const.h"
#include "oservercore.h"

//标准输入和标准输出
QTextStream cin(stdin);
QTextStream cout(stdout);

int main(int argc, char *argv[])
{
#ifdef WHTSKY_DEBUG
    //方便Windows下调试
    QString dir_name = QDir::current().dirName();
    if(dir_name == "release" || dir_name == "debug")
        QDir::setCurrent("..");
#endif // WHTSKY_DEBUG
        
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    OServerCore sc;
    sc.run();

    return a.exec();
}
