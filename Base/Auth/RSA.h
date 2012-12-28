#ifndef ZEROMS_BASE_AUTH_RSA_H
#define ZEROMS_BASE_AUTH_RSA_H

#include <QPair>

namespace ZeroMS {
namespace Base {
namespace Auth {

class RSAKeyMaker;

class RSAKey
{
    friend class RSAKeyMaker;
public:
    enum PublicExp
    {
        RSA3,RSAF4
    };

    RSAKey();
    RSAKey(const RSAKey &other);
    RSAKey &operator =(const RSAKey &other);
    virtual ~RSAKey();

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

}}}   // namespace ZeroMS::Base::Auth

#endif // ZEROMS_BASE_AUTH_RSA_H
