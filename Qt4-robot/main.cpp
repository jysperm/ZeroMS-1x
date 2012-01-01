#include <QCoreApplication>
#include <QTextCodec>
#include "const.h"
#include "orobot.h"

//声明客户端通讯核心(扩展)
ORobot *robot;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    robot=new ORobot;
    robot->init();

    return a.exec();
}
