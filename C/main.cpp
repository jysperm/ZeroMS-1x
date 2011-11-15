#include <QApplication>
#include <QTextCodec>
#include "const.h"
#include "clientcoreex.h"

//声明客户端通讯核心(扩展)
ClientCoreEx *cc;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    cc=new ClientCoreEx(&a);
    cc->init();

    return a.exec();
}
