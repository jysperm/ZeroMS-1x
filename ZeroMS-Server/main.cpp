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

/*
大概思路：
在main函数中先创建Qt的QCoreApplication以便建立事件循环
然后声明一个OServerCore对象，由它接管全部服务器功能。

OServerCore中以成员的方式存在一个OProtocolForCS类的实例，
该类接管所有协议相关的细节问题，例如各个参数应该以怎样的格式在网络中传输。
每个需要发送的消息在其中表现为一个函数。
OProtocolForCS类还负责分拣消息，当收到消息后，它会按照协议拆分消息中的参数，
然后发射信号，信号由OServerCore接收，完成应有的逻辑

OClient类抽象出一个客户端。自己有自己的连接对象和数据缓冲
(因为SB2.x支持多连接，所以连接对象和数据缓冲都不止一个).
自己负责接收自己的数据，当数据缓冲中达到一个完整的消息之后，
发射信号，由OProtocolForCS接收并进行分拣。

OClient::Connect类抽象一个客户端连接(SB2.x是允许多连接的),
这个连接可以是主连接或者次要连接。
在大多数情况下，我们使用一个OClient::Connect对象来表示需要服务的对象

OServerCore是整个服务器的核心，用于连接其他各部分
*/
/*
公共部分：
OGloal不是一个类，只是一组头文件，
其中是一些公共的小函数，以及协议相关的常量定义

OMessage类抽象一个消息数据包
提供了解析消息的能力，按照协议中的规定提取一些与具体消息类型无关的参数
(有关的是由OProtocolForCS提取的)

OSetting类用于管理配置文件
是对QSetting的一个简单封装，是从零毫秒1.x中直接复制过来的
*/
