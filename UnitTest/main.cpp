#include <QApplication>
#include <QtTest>

#include "Base/Auth/Hash.h"
#include "Base/Auth/RSA.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
#ifdef QT_GUI_LIB
    QTEST_DISABLE_KEYPAD_NAVIGATION
#endif

    ZeroMS::UnitTest::Base::Auth::HashTest hashTest;
    ZeroMS::UnitTest::Base::Auth::RSATest rsaTest;

    QTest::qExec(&hashTest, argc, argv);
    QTest::qExec(&rsaTest, argc, argv);

    return 0;
}
