#ifndef ZEROMS_BASE_AUTH_RSA_H
#define ZEROMS_BASE_AUTH_RSA_H

#include <QPair>

namespace ZeroMS {
namespace Base {
namespace Auth {

class RSA;

class RSAKey
{
    friend class RSA;
public:
    enum PublicExp
    {
        RSA3,RSAF4
    };

    RSAKey();
    RSAKey(const RSAKey &other);
    virtual ~RSAKey();

    void print(FILE *file);
protected:
    class RSAKeyPrivate;
    RSAKey(RSAKeyPrivate *key);
private:
    RSAKeyPrivate *data;
};

class RSAPrivateKey : public RSAKey
{
    friend class RSA;
private:
    RSAPrivateKey(RSAKeyPrivate *rsa):RSAKey(rsa){}
};

class RSAPublicKey : public RSAKey
{
    friend class RSA;
private:
    RSAPublicKey(RSAKeyPrivate *rsa):RSAKey(rsa){}
};

class RSA
{
public:
    static QPair<RSAPrivateKey,RSAPublicKey> makeKeyPair(int bits=2048,RSAKey::PublicExp publicExp=RSAKey::RSAF4);
private:
    static unsigned long getPublicExp(RSAKey::PublicExp publicExp);
};

}}}   // namespace ZeroMS::Base::Auth

#endif // ZEROMS_BASE_AUTH_RSA_H
