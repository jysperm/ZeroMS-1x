#include <fstream>
#include <QTextStream>
#include <openssl/rsa.h>
#include "RSA.h"

namespace ZeroMS {
namespace Base {
namespace Auth {

class RSAKey::RSAKeyPrivate
{
public:
    ::RSA *data;
};

RSAKey::RSAKey()
{
    this->data=new RSAKeyPrivate;
    this->data->data=RSA_new();
}

RSAKey::RSAKey(const RSAKey &other)
{
    this->data=new RSAKeyPrivate;
    this->data->data=other.data->data;
    RSA_up_ref(this->data->data);
}

RSAKey::RSAKey(RSAKeyPrivate *key)
{
    this->data=new RSAKeyPrivate;
    this->data->data=key->data;
}

RSAKey::~RSAKey()
{
    RSA_free(this->data->data);
    delete this->data;
}

void RSAKey::print(FILE *file)
{
    RSA_print_fp(file,this->data->data,0);
}

QPair<RSAPrivateKey,RSAPublicKey> RSA::makeKeyPair(int bits,RSAKey::PublicExp publicExp)
{
    ::RSA *rsa=RSA_generate_key(bits,getPublicExp(publicExp),NULL,NULL);
    ::RSA *publicRsa=RSAPublicKey_dup(rsa);
    ::RSA *privateRsa=RSAPrivateKey_dup(rsa);

    RSA_free(rsa);

    RSAKey::RSAKeyPrivate *ppublicRsa=new RSAKey::RSAKeyPrivate;
    ppublicRsa->data=publicRsa;
    RSAKey::RSAKeyPrivate *pprivateRsa=new RSAKey::RSAKeyPrivate;
    pprivateRsa->data=privateRsa;

    return qMakePair(RSAPrivateKey(ppublicRsa),RSAPublicKey(pprivateRsa));
}

unsigned long RSA::getPublicExp(RSAKey::PublicExp publicExp)
{
    switch(publicExp)
    {
    case RSAKey::RSA3:
        return RSA_3;
    case RSAKey::RSAF4:
        return RSA_F4;
    }
    return 0;
}

}}}   // namespace ZeroMS::Base::Auth
