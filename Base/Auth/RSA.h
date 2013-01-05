#ifndef ZEROMS_BASE_AUTH_RSA_H
#define ZEROMS_BASE_AUTH_RSA_H

#include <QPair>
#include "Base/Exception.h"

#ifndef Q_QDOC
namespace ZeroMS {
namespace Base {
namespace Auth {
#endif

class RSAKeyMaker;

class RSAKey
{
    friend class RSAKeyMaker;
public:
    enum PublicExp
    {
        RSA3,RSAF4
    };

    class InvalidKeyException : public LogicException
    {
    public:
        explicit InvalidKeyException():LogicException(""){}
    };

    RSAKey();
    RSAKey(const RSAKey &other);
    RSAKey &operator =(const RSAKey &other);
    virtual ~RSAKey();

    int size();
    QString print();
protected:
    class RSAKeyPrivate;
    RSAKey(RSAKeyPrivate *key);
    RSAKeyPrivate *data;
};

class RSAPrivateKey : public RSAKey
{
    friend class RSAKeyMaker;
public:
    RSAPrivateKey():RSAKey(){}
    QByteArray toPEM(QString passwd="passwd");
    bool isValid();

    QByteArray encrypt(QByteArray data);
    QByteArray decrypt(QByteArray data);

    static RSAPrivateKey fromPEM(QByteArray pem,QString passwd="passwd");
private:
    RSAPrivateKey(RSAKeyPrivate *rsa):RSAKey(rsa){}
};

class RSAPublicKey : public RSAKey
{
    friend class RSAKeyMaker;
public:
    RSAPublicKey():RSAKey(){}
    QByteArray toPEM();

    QByteArray encrypt(QByteArray data);
    QByteArray decrypt(QByteArray data);

    static RSAPublicKey fromPEM(QByteArray pem);
private:
    RSAPublicKey(RSAKeyPrivate *rsa):RSAKey(rsa){}
};

class RSAKeyMaker
{
public:
    static QPair<RSAPrivateKey,RSAPublicKey> makeKeyPair(int bits=2048,RSAKey::PublicExp publicExp=RSAKey::RSAF4);
private:
    static unsigned long getPublicExp(RSAKey::PublicExp publicExp);
};

#ifndef Q_QDOC
}}}   // namespace ZeroMS::Base::Auth
#endif

#ifdef QT_TESTLIB_LIB

#include <QObject>

namespace ZeroMS {
namespace UnitTest {
namespace Base {
namespace Auth {

class RSATest : public QObject
{
    Q_OBJECT
public:
    RSATest();
private slots:
    void testRSA();
};

}}}}    // namespace ZeroMS::UnitTest::Base::Auth

#endif  // QT_TESTLIB_LIB

#endif // ZEROMS_BASE_AUTH_RSA_H
