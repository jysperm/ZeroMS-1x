#include <QDebug>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
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

RSAKey &RSAKey::operator =(const RSAKey &other)
{
    if(this==&other)
        return *this;

    RSA_free(this->data->data);
    this->data->data=other.data->data;
    RSA_up_ref(this->data->data);

    return *this;
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

QString RSAKey::print()
{
    BIO *bio=BIO_new(BIO_s_mem());
    RSA_print(bio,this->data->data,0);

    int len=BIO_ctrl_pending(bio);
    char *out=new char[len];
    BIO_read(bio,out,len);

    QByteArray ba(out,len);
    delete[] out;
    BIO_free(bio);

    return ba;
}

QByteArray RSAPrivateKey::toPEM(QString passwd)
{
    BIO *bio=BIO_new(BIO_s_mem());

    //TODO:调研去掉下一行是否可以正常工作
    OpenSSL_add_all_algorithms();

    EVP_PKEY *evpkey=EVP_PKEY_new();
    EVP_PKEY_assign_RSA(evpkey,this->data->data);
    RSA_up_ref(this->data->data);

    PEM_write_bio_PrivateKey(bio,
                             evpkey,EVP_des_ede3_cbc(),
                             NULL,
                             0,
                             NULL,
                             reinterpret_cast<void*>(passwd.toUtf8().data()));

    int len=BIO_ctrl_pending(bio);
    char *out=new char[len];
    BIO_read(bio,out,len);

    QByteArray ba(out,len);
    delete[] out;
    EVP_PKEY_free(evpkey);
    BIO_free(bio);

    return ba;
}

bool RSAPrivateKey::isValid()
{
    return RSA_check_key(this->data->data);
}

RSAPrivateKey RSAPrivateKey::fromPEM(QByteArray pem,QString passwd)
{
    BIO *bio=BIO_new(BIO_s_mem());

    //TODO:调研去掉下一行是否可以正常工作
    OpenSSL_add_all_algorithms();

    BIO_write(bio,pem.constData(),pem.size());

    EVP_PKEY *evpkey=PEM_read_bio_PrivateKey(bio,NULL,NULL,reinterpret_cast<void*>(passwd.toUtf8().data()));

    ::RSA *rsa=EVP_PKEY_get1_RSA(evpkey);

    ::RSA *priRsa=RSAPrivateKey_dup(rsa);

    RSAKeyPrivate *pKey=new RSAKeyPrivate;
    pKey->data=priRsa;

    EVP_PKEY_free(evpkey);
    BIO_free(bio);
    RSA_free(rsa);

    return RSAPrivateKey(pKey);
}

QByteArray RSAPublicKey::toPEM()
{
    BIO *bio=BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPublicKey(bio,this->data->data);

    int len=BIO_ctrl_pending(bio);
    char *out=new char[len];
    BIO_read(bio,out,len);

    QByteArray ba(out,len);
    delete[] out;
    BIO_free(bio);

    return ba;
}

RSAPublicKey RSAPublicKey::fromPEM(QByteArray pem)
{
    BIO *bio=BIO_new(BIO_s_mem());

    BIO_write(bio,pem.constData(),pem.size());

    qDebug()<<BIO_ctrl_pending(bio);

    ::RSA *rsa=PEM_read_bio_RSAPublicKey(bio,NULL,NULL,NULL);

    ::RSA *pubRsa=RSAPublicKey_dup(rsa);

    RSAKeyPrivate *pKey=new RSAKeyPrivate;
    pKey->data=pubRsa;

    BIO_free(bio);
    RSA_free(rsa);

    return RSAPublicKey(pKey);
}

QPair<RSAPrivateKey,RSAPublicKey> RSAKeyMaker::makeKeyPair(int bits,RSAKey::PublicExp publicExp)
{
    ::RSA *rsa=RSA_generate_key(bits,getPublicExp(publicExp),NULL,NULL);
    ::RSA *publicRsa=RSAPublicKey_dup(rsa);
    ::RSA *privateRsa=RSAPrivateKey_dup(rsa);

    RSA_free(rsa);

    RSAKey::RSAKeyPrivate *ppublicRsa=new RSAKey::RSAKeyPrivate;
    ppublicRsa->data=publicRsa;
    RSAKey::RSAKeyPrivate *pprivateRsa=new RSAKey::RSAKeyPrivate;
    pprivateRsa->data=privateRsa;

    auto pair=qMakePair(RSAPrivateKey(pprivateRsa),RSAPublicKey(ppublicRsa));

    delete ppublicRsa;
    delete pprivateRsa;

    return pair;
}

unsigned long RSAKeyMaker::getPublicExp(RSAKey::PublicExp publicExp)
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
