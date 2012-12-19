#ifndef ZEROMS_UNITTEST_TESTTOOLS_H
#define ZEROMS_UNITTEST_TESTTOOLS_H

#include <QString>

namespace ZeroMS {
namespace UnitTest {

class TestTools
{
public:
    TestTools();
    static QString randString(int length);
    static QByteArray toQByteArray(const QString str);
};

}}  // ZeroMS::UnitTest

#endif // ZEROMS_UNITTEST_TESTTOOLS_H
