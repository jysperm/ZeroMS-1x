#include <QtCore>
#include <QtTest>
#include <openssl/md5.h>
#include <openssl/sha.h>

#include "RSA.h"
#include "UnitTest/TestTools.h"

namespace ZeroMS {
namespace UnitTest {
namespace Base {
namespace Auth {

RSATest::RSATest()
{

}

void RSATest::testRSA()
{
    using namespace ::ZeroMS::Base::Auth;
    qsrand(QTime::currentTime().second() * QTime::currentTime().msec());

    auto pair=RSAKeyMaker::makeKeyPair(1024);

    RSAPrivateKey priKey=pair.first;
    RSAPublicKey pubKey=pair.second;

    QCOMPARE(priKey.size(),1024/8);
    QCOMPARE(pubKey.size(),1024/8);

    QCOMPARE(priKey.isValid(),true);

    RSAPrivateKey nonValid;
    QCOMPARE(nonValid.isValid(),false);

    QCOMPARE(priKey.print().size()>1000,true);

    QByteArray data;
    data.append(TestTools::randString(qrand() % 100).repeated(qrand() % 1000));

    QByteArray ePriData=priKey.encrypt(data);
    QByteArray ePubData=pubKey.encrypt(data);

    QString passwd=TestTools::randString(30);

    QByteArray bPriKey=priKey.toPEM(passwd);
    QByteArray bPubKey=pubKey.toPEM();

    priKey=RSAPrivateKey::fromPEM(bPriKey,passwd);
    pubKey=RSAPublicKey::fromPEM(bPubKey);

    QCOMPARE(pubKey.decrypt(ePriData),data);
    QCOMPARE(priKey.decrypt(ePubData),data);
}

}}}}    // namespace ZeroMS::UnitTest::Base::Auth


