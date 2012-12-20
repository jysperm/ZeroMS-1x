#include <QtCore>
#include <QtTest>
#include <openssl/md5.h>
#include <openssl/sha.h>

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
    QCOMPARE(MD5::md5(TestTools::toQByteArray(text)),
             QByteArray::fromHex(TestTools::toQByteArray(hash)));
}

void HashTest::testMD5_MD5()
{
    using namespace ZeroMS::Base::Auth;

    MD5 md5;

    QString total;

    for(int i=0;i<10;i++)
    {
        QString str=TestTools::randString(i*100);

        total.append(str);

        QByteArray bStr;
        bStr.append(str);

        md5.append(bStr);
    }

    QCOMPARE(md5.result(),MD5::md5(TestTools::toQByteArray(total)));
}

void HashTest::testSHA_sha()
{
    using namespace ZeroMS::Base::Auth;

    QFETCH(QString,text);
    QFETCH(QString,sha1);
    QFETCH(QString,sha224);
    QFETCH(QString,sha256);
    QFETCH(QString,sha384);
    QFETCH(QString,sha512);

    QCOMPARE(SHA::sha(text,SHA::SHA1),sha1);
    QCOMPARE(SHA::sha(text,SHA::SHA224),sha224);
    QCOMPARE(SHA::sha(text,SHA::SHA256),sha256);
    QCOMPARE(SHA::sha(text,SHA::SHA384),sha384);
    QCOMPARE(SHA::sha(text,SHA::SHA512),sha512);

    QCOMPARE(SHA::sha(TestTools::toQByteArray(text),SHA::SHA1),
             QByteArray::fromHex(TestTools::toQByteArray(sha1)));
    QCOMPARE(SHA::sha(TestTools::toQByteArray(text),SHA::SHA224),
             QByteArray::fromHex(TestTools::toQByteArray(sha224)));
    QCOMPARE(SHA::sha(TestTools::toQByteArray(text),SHA::SHA256),
             QByteArray::fromHex(TestTools::toQByteArray(sha256)));
    QCOMPARE(SHA::sha(TestTools::toQByteArray(text),SHA::SHA384),
             QByteArray::fromHex(TestTools::toQByteArray(sha384)));
    QCOMPARE(SHA::sha(TestTools::toQByteArray(text),SHA::SHA512),
             QByteArray::fromHex(TestTools::toQByteArray(sha512)));
}

void HashTest::testSHA_SHA()
{
    using namespace ZeroMS::Base::Auth;

    SHA sha1(SHA::SHA1);
    SHA sha224(SHA::SHA224);
    SHA sha256(SHA::SHA256);
    SHA sha384(SHA::SHA384);
    SHA sha512(SHA::SHA512);

    QString total;

    for(int i=0;i<10;i++)
    {
        QString str=TestTools::randString(i*100);

        total.append(str);

        QByteArray bStr=TestTools::toQByteArray(str);

        sha1.append(bStr);
        sha224.append(bStr);
        sha256.append(bStr);
        sha384.append(bStr);
        sha512.append(bStr);
    }

    QByteArray bTotal=TestTools::toQByteArray(total);

    QCOMPARE(sha1.result(),SHA::sha(bTotal,SHA::SHA1));
    QCOMPARE(sha224.result(),SHA::sha(bTotal,SHA::SHA224));
    QCOMPARE(sha256.result(),SHA::sha(bTotal,SHA::SHA256));
    QCOMPARE(sha384.result(),SHA::sha(bTotal,SHA::SHA384));
    QCOMPARE(sha512.result(),SHA::sha(bTotal,SHA::SHA512));
}

void HashTest::testMD5_md5_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("hash");

    QTest::newRow("English")<<"jybox"<<"5932827d7c7ec77eaad3115dda7607ec";
    QTest::newRow("English")<<"Qt is a cross-platform application and UI framework for developers using C++ or QML, a CSS & JavaScript like language. Qt Creator is the supporting Qt IDE."
                            <<"40c8ab0a5165b4e83b2190df8e0ddc13";

    QTest::newRow("Chinese")<<"精英王子-jybox"<<"6c421b5a8df96d92459ebe8836af75c9";
    QTest::newRow("Chinese")<<"分析报告称，根据各企业对Linux 3.0新代码的贡献来衡量，微软是继红帽、英特尔、Novell和IBM之后的第五大Linux 3.0企业支持者。"
                            <<"aa64ff6e54276c0c63fdd99585075554";

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

void HashTest::testSHA_sha_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("sha1");
    QTest::addColumn<QString>("sha224");
    QTest::addColumn<QString>("sha256");
    QTest::addColumn<QString>("sha384");
    QTest::addColumn<QString>("sha512");

    QTest::newRow("English")<<"jybox"<<"84784aa06ef166742e546c5f65642d95c6e95892"
                            <<"4da591f87c7870a84a507a7441bc1a1763c3b8fcb24c85403ab7b610"
                            <<"a9af99a4bde423121f626c8c214aed71c873ff80e7f309ef6c87d6ae40c4a1fd"
                            <<"abd1e6761774fb306937f671ab1e834905a8a7b0abb2639c779dc0169df489e8eab9b3ab1f736e0e9a9efde0e6cb46bb"
                            <<"3482862da51831f9d72abb19c5ce556678a6630b3db2e9b04ea73afb5063d1daff05218dc01684f357b20dc8b15412590d054e72bcf2519665f17add0ddbc81c";

    QTest::newRow("English")<<"Qt is a cross-platform application and UI framework for developers using C++ or QML, a CSS & JavaScript like language. Qt Creator is the supporting Qt IDE."
                            <<"9ca173053c732facbb973e61fc706c2a51cd05ed"
                            <<"659ced4b631c4dc795e2d1bca6bdd5d04a8d6f910e0394109abdbadd"
                            <<"5acc2a2e782ebe616858b4fcdf5b9d4c3df43b2ba3b2218c2ad331d01cf4d378"
                            <<"d37b0854b345f60d7aab6b1f0209b14ccb07458cc64512998494d6fd6ea68903c2908866db55eb56e81cb6b99f306159"
                            <<"4ab28415739270dbe72ec0372ffdc2e54d4416b37d7987b9b7d2e62be399bbd46d4b9b1950c884f55124fd8d1d9ec9f75061dfc9598f048f01a366d04a6312cb";

    QTest::newRow("Chinese")<<"精英王子-jybox"<<"9b5b455106045add0e33e25db7a9b3a444e37d7c"
                            <<"ee967a720978cb7e3f2fdfe6f43e9962bb41501797bc44518d993b67"
                            <<"c8b035a36e06f155f161f7fcf14536913a5626ed63c2ab5231885203d2696c26"
                            <<"049876bf2657f441039a2c595305e6ca58ef0962c8c5eca6baa9a1b5ba8471e6837a7c93eed75165b6325897ba496cc9"
                            <<"43155612abb06d5a4543a24e888e2f4fdc66492419424833533693997ba0b643d60ee9327ef78b11296db750dd59067685015c2abba869585919fce9060fe364";

    QTest::newRow("Chinese")<<"分析报告称，根据各企业对Linux 3.0新代码的贡献来衡量，微软是继红帽、英特尔、Novell和IBM之后的第五大Linux 3.0企业支持者。"
                            <<"677ddb49e779e49aa8ad3ccb6033f84e0cdf509f"
                            <<"2631a6d4a005d57dca24bed6ac821221e6b5c149cb267f3dc0ce0d61"
                            <<"bef6d03897644bd3717b4765a15ce6611d69a64d6a1757ed7f8f0aad1ccaefc5"
                            <<"f8245483e5122dab150c8148316503f196be4e0a465955bb30828a481436f3843860492d42173c6cf70c55726a3b8e30"
                            <<"9e5581c4bc35682d1800bde98a9f4fd6aaeff7eaa0e5f6a97cd563df45af93abd6cf71917979ee33d821ae83f4ad30cb1546f12cad47e848ec54e7cfc0a04f77";

    QTest::newRow("Long String")<<QString("long data 长数据").repeated(1000)<<"f3c0dcbac5a73f7443a091c8ea0378655d14e44e"
                                <<"ab8fc061e7f02e0ef3ffb4fd9ae2196d1ddad3ae9b749e6d22eb2991"
                                <<"332829ca6e49e7348a9ec2406dc6c6b4b82ef10727ce1ad6b2bbed2e38ffa40e"
                                <<"cea9fe49039e6a742cf4b8246f0b30d3899b5f3e411ed9c8094cec54be2dd3cc2f110470b8155e6cdf79514a6d14efd8"
                                <<"7bee7b0e1d589c65df29f6dda3a5ec7e76da1a47a9afc658b85ed86f197b0b3539d3e6c88840ae6b05061b54432d23acb19844e841059b372f57c1e7fd2dd991";

    for(int i=0;i<10;i++)
    {
        QString str=TestTools::randString(i*100);
        unsigned char out1[SHA_DIGEST_LENGTH];
        unsigned char out224[SHA224_DIGEST_LENGTH];
        unsigned char out256[SHA256_DIGEST_LENGTH];
        unsigned char out384[SHA384_DIGEST_LENGTH];
        unsigned char out512[SHA512_DIGEST_LENGTH];

        auto cucData=reinterpret_cast<const unsigned char*>(str.toUtf8().constData());

        ::SHA1(cucData,str.toUtf8().size(),out1);
        ::SHA224(cucData,str.toUtf8().size(),out224);
        ::SHA256(cucData,str.toUtf8().size(),out256);
        ::SHA384(cucData,str.toUtf8().size(),out384);
        ::SHA512(cucData,str.toUtf8().size(),out512);

        QTest::newRow("Random String")<<str
                                      <<QString(QByteArray(reinterpret_cast<const char*>(out1),SHA_DIGEST_LENGTH).toHex())
                                      <<QString(QByteArray(reinterpret_cast<const char*>(out224),SHA224_DIGEST_LENGTH).toHex())
                                      <<QString(QByteArray(reinterpret_cast<const char*>(out256),SHA256_DIGEST_LENGTH).toHex())
                                      <<QString(QByteArray(reinterpret_cast<const char*>(out384),SHA384_DIGEST_LENGTH).toHex())
                                      <<QString(QByteArray(reinterpret_cast<const char*>(out512),SHA512_DIGEST_LENGTH).toHex());
    }
}

}}}}    // namespace ZeroMS::UnitTest::Base::Auth

