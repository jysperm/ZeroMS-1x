#include <QtCore>
#include <QtTest>
#include <openssl/md5.h>

#include "Hash.h"
#include "UnitTest/TestTools.h"

namespace ZeroMS {
namespace UnitTest {
namespace Base {
namespace Auth {

HashTest::HashTest()
{

}

void HashTest::testMD5_md5()
{
    using namespace ZeroMS::Base::Auth;

    QFETCH(QString,text);
    QFETCH(QString,hash);

    QCOMPARE(MD5::md5(text),hash);
}

void HashTest::testSHA()
{

}

void HashTest::testMD5_md5_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("hash");

    QTest::newRow("English")<<"jybox"<<"5932827d7c7ec77eaad3115dda7607ec";
    QTest::newRow("English")<<"Qt is a cross-platform application and UI framework for developers using C++ or QML, a CSS & JavaScript like language. Qt Creator is the supporting Qt IDE."<<
                              "40c8ab0a5165b4e83b2190df8e0ddc13";

    QTest::newRow("Chinese")<<"精英王子-jybox"<<"6c421b5a8df96d92459ebe8836af75c9";
    QTest::newRow("Chinese")<<"分析报告称，根据各企业对Linux 3.0新代码的贡献来衡量，微软是继红帽、英特尔、Novell和IBM之后的第五大Linux 3.0企业支持者。"<<
                         "aa64ff6e54276c0c63fdd99585075554";

    QTest::newRow("Long String")<<QString("long data 长数据").repeated(1000)<<"a0d447d149b99c8161c196ed16734b8d";

    for(int i=0;i<10;i++)
    {
        QString str=TestTools::randString(i*100);
        unsigned char out[MD5_DIGEST_LENGTH];
        ::MD5(reinterpret_cast<const unsigned char*>(str.toUtf8().constData()),
            str.toUtf8().size(),
            out);
        QString result=QByteArray(reinterpret_cast<const char*>(out),MD5_DIGEST_LENGTH).toHex();

        QTest::newRow("Random String")<<str<<result;
    }
}

}}}}    // namespace ZeroMS::UnitTest::Base::Auth

