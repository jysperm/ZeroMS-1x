#include <QApplication>
#include <QDir>
#include <QTextCodec>
#include "const.h"
#include "oclientcoreex.h"

//声明客户端通讯核心(扩展)
OClientCoreEx *cc;

int main(int argc, char *argv[])
{
    //方便Windows下调试
    QString dir_name = QDir::current().dirName();
    if(dir_name == "release" || dir_name == "debug")
        QDir::setCurrent("..");

    QApplication a(argc, argv);

    a.setQuitOnLastWindowClosed(0);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    cc=new OClientCoreEx;
    cc->init();

    return a.exec();
}
