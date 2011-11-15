#include <QCoreApplication>
#include <QTextStream>
#include <QTextCodec>
#include "const.h"
#include "oservercore.h"

//标准输入和标准输出
QTextStream cin(stdin);
QTextStream cout(stdout);

//程序模块：
//-主程序(Main)，除其他模块的内容都算主程序，主要包括const.h、main.cpp
//-客户端封装(ClientConn)，封装了客户端连接对象，只是加了几个公有变量，包括clientconn.h
//-连接对象表(SocketList),用来保存和维护连接对象，主要包括socketlist.h、socketlist.cpp
//-通讯核心(ServerCore)，主体程序，用来处理消息，主要包括servercore.h、servercore.cpp
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    OServerCore sc;
    sc.run();

    return a.exec();
}
