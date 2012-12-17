#include <QtTest>

#include "../Base/Auth/Hash.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
#ifdef QT_GUI_LIB
    QTEST_DISABLE_KEYPAD_NAVIGATION
#endif

    ZeroMS::UnitTest::Base::Auth::HashTest hashTest;

    QTest::qExec(&hashTest, argc, argv);

    return 0;
}
