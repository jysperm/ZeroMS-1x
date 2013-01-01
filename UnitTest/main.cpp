#include <QApplication>
#include <QtTest>

#if QT_VERSION < 0x050000
#include <QTextCodec>
#endif

#include "Base/Auth/Hash.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    QApplication app(argc, argv);
#ifdef QT_GUI_LIB
    QTEST_DISABLE_KEYPAD_NAVIGATION
#endif

    ZeroMS::UnitTest::Base::Auth::HashTest hashTest;

    QTest::qExec(&hashTest, argc, argv);

    return 0;
}
