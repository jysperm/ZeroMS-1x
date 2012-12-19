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
    QString str;
    qsrand(QTime::currentTime().second() * QTime::currentTime().msec());
    for(int i=0;i<length;i++)
    {
        str.append(charList.at(qrand() % charList.length()));
    }

    return str;
}

QByteArray TestTools::toQByteArray(const QString str)
{
    QByteArray ba;
    ba.append(str);
    return ba;
}

}}  // ZeroMS::UnitTest
