#include <QTime>
#include "TestTools.h"

namespace ZeroMS {
namespace UnitTest {

TestTools::TestTools()
{
}

QString TestTools::randString(int length)
{
    QString charList = QString("0123456789ABCDEFGHIJKLMNOPQRSTUVWZYZ");
    QString str = QString();
    QTime t;
    t= QTime::currentTime();
    qsrand(QTime::currentTime().second() * QTime::currentTime().msec());
    for(int i=0;i<length;i++)
    {
        str.append(charList.at(qrand() % charList.length()));
    }

    return str;
}

}}  // ZeroMS::UnitTest
