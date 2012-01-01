#include <QEventLoop>
#include "orobot.h"

ORobot::ORobot()
{

}

void ORobot::init()
{
    QEventLoop waitConnected;
    connect(this,SIGNAL(onConnected()),&waitConnected,SLOT(quit()));
    connectTo("127.0.0.1",4321);
    waitConnected.exec();
    if(conn->state()!=QTcpSocket::ConnectedState)
    {
        exit(1);
    }

    QEventLoop waitTimeUp;

    connect(this,SIGNAL(onTimeChange(uint)),&waitTimeUp,SLOT(quit()));
    msgAskTime();
    waitTimeUp.exec();

    QString uname=tr("orobot");
    QString pwd=tr("789456");
    pwd=md5(pwd);
    msgLoginNoTime(uname,pwd);
}

void ORobot::msgLoginOk(OPacket &packet)
{
    connect(this,SIGNAL(onSMsg(QString,QString,QString)),this,SLOT(MSG(QString,QString,QString)));
}

void ORobot::MSG(QString user,QString view,QString msg)
{
    msgCMsg(view,tr("回复")+msg);
}

