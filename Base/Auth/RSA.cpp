#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include "RSA.h"

namespace ZeroMS {
namespace Base {
namespace Auth {

/*!
    \class RSAKey
    \brief RSA密钥基类

    RSAKey的储存着一个RSA密钥.
*/

/*!
    \enum RSAKey::PublicExp

    该枚举指定了RSA密钥的指数类型.

    \value RSA3
           0x3L
    \value RSAF4
           0x10001L
*/

/*!
    \class RSAKey::InvalidKeyException
    \brief RSA密钥错误异常

    当RSA密钥错误时使用.
*/

/*
    纯粹用于包装来自openssl的RSA
*/

class RSAKey::RSAKeyPrivate
{
public:
    ::RSA *rsa;
};

/*!
    构造一个RSA密钥.
*/

RSAKey::RSAKey()
{
    this->data=new RSAKeyPrivate;
    this->data->rsa=RSA_new();
}

/*!
    从一个已有的RSA密钥( \a other )复制构造.
*/

RSAKey::RSAKey(const RSAKey &other)
{
    this->data=new RSAKeyPrivate;
    this->data->rsa=other.data->rsa;
    RSA_up_ref(this->data->rsa);
}

/*!
    从一个已有的RSA密钥( \a other )赋值.
*/

RSAKey &RSAKey::operator =(const RSAKey &other)
{
    if(this==&other)
        return *this;

    RSA_free(this->data->rsa);
    this->data->rsa=other.data->rsa;
    RSA_up_ref(this->data->rsa);

    return *this;
}

/*
    从RSAKeyPrivate中的RSA结构构造
    \a key 的所有权将被转移到该类.
*/

RSAKey::RSAKey(RSAKeyPrivate *key)
{
    this->data=new RSAKeyPrivate;
    this->data->rsa=key->rsa;
}

/*!
    析构函数
*/

RSAKey::~RSAKey()
{
    RSA_free(this->data->rsa);
    delete this->data;
}

/*!
    获取RSA密钥的长度，单位字节.
*/

int RSAKey::size()
{
    if(!this->data->rsa->n)
        throw InvalidKeyException();
    return RSA_size(this->data->rsa);
}

/*!
    以文本的格式返回密钥的信息.
*/

QString RSAKey::print()
{
    BIO *bio=BIO_new(BIO_s_mem());
    RSA_print(bio,this->data->rsa,0);

    int len=BIO_ctrl_pending(bio);
    char *out=new char[len];
    BIO_read(bio,out,len);

    QByteArray ba(out,len);
    delete[] out;
    BIO_free(bio);

    return ba;
}

/*!
    \class RSAPrivateKey
    \brief RSA私钥

    RSAKey的储存着一个RSA私钥.
*/

/*!
    转换为PEM格式， \a passwd 为密码.

    \sa fromPEM()
*/

QByteArray RSAPrivateKey::toPEM(QString passwd)
{
    BIO *bio=BIO_new(BIO_s_mem());

    //TODO:调研去掉下一行是否可以正常工作
    OpenSSL_add_all_algorithms();

    EVP_PKEY *evpkey=EVP_PKEY_new();
    EVP_PKEY_assign_RSA(evpkey,this->data->rsa);
    RSA_up_ref(this->data->rsa);

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

/*!
    检查私钥是否有效.
*/

bool RSAPrivateKey::isValid()
{
    if(!this->data->rsa->n)
        return false;
    return RSA_check_key(this->data->rsa);
}

/*!
    私钥加密(\a data), 即签名.

    \sa RSAPublicKey::decrypt()
*/

QByteArray RSAPrivateKey::encrypt(QByteArray data)
{
    int rsaSize=this->size();
    int blen=rsaSize-11;
    int blocks=data.size()/blen + 1;

    QByteArray result;

    for(int i=0;i<blocks;i++)
    {
        unsigned char *from=i*blen+reinterpret_cast<unsigned char*>(data.data());
        unsigned char *to=new unsigned char[rsaSize];

        int flen;
        if(i!=blocks-1)
            flen=blen;
        else
            flen=data.size()%blen;

        RSA_private_encrypt(flen,from,to,this->data->rsa,RSA_PKCS1_PADDING);

        result.append(reinterpret_cast<const char*>(to),rsaSize);
        delete [] to;
    }

    return result;
}

/*!
    私钥解密(\a data), 即解密.

    \sa RSAPublicKey::encrypt()
*/

QByteArray RSAPrivateKey::decrypt(QByteArray data)
{
    int rsaSize=this->size();
    int blocks=data.size()/rsaSize;

    QByteArray result;

    for(int i=0;i<blocks;i++)
    {
        unsigned char *from=i*rsaSize+reinterpret_cast<unsigned char*>(data.data());
        unsigned char *to=new unsigned char[rsaSize];

        int len=RSA_private_decrypt(rsaSize,from,to,this->data->rsa,RSA_PKCS1_PADDING);

        result.append(reinterpret_cast<const char*>(to),len);
        delete [] to;
    }

    return result;
}

/*!
    从PEM格式创建实例， \a passwd 为密码.

    \sa toPEM()
*/

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
    pKey->rsa=priRsa;

    EVP_PKEY_free(evpkey);
    BIO_free(bio);
    RSA_free(rsa);

    return RSAPrivateKey(pKey);
}

/*!
    \class RSAPublicKey
    \brief RSA公钥

    RSAKey的储存着一个RSA公钥.
*/

/*!
    转换为PEM格式.

    \sa fromPEM()
*/

QByteArray RSAPublicKey::toPEM()
{
    BIO *bio=BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPublicKey(bio,this->data->rsa);

    int len=BIO_ctrl_pending(bio);
    char *out=new char[len];
    BIO_read(bio,out,len);

    QByteArray ba(out,len);
    delete[] out;
    BIO_free(bio);

    return ba;
}

/*!
    公钥加密(\a data), 即加密.

    \sa RSAPrivateKey::decrypt()
*/

QByteArray RSAPublicKey::encrypt(QByteArray data)
{
    int rsaSize=this->size();
    int blen=rsaSize-11;
    int blocks=data.size()/blen + 1;

    QByteArray result;

    for(int i=0;i<blocks;i++)
    {
        unsigned char *from=i*blen+reinterpret_cast<unsigned char*>(data.data());
        unsigned char *to=new unsigned char[rsaSize];

        int flen;
        if(i!=blocks-1)
            flen=blen;
        else
            flen=data.size()%blen;

        RSA_public_encrypt(flen,from,to,this->data->rsa,RSA_PKCS1_PADDING);

        result.append(reinterpret_cast<const char*>(to),rsaSize);
        delete [] to;
    }

    return result;
}

/*!
    公钥解密(\a data), 即验签.

    \sa RSAPrivateKey::encrypt()
*/

QByteArray RSAPublicKey::decrypt(QByteArray data)
{
    int rsaSize=this->size();
    int blocks=data.size()/rsaSize;

    QByteArray result;

    for(int i=0;i<blocks;i++)
    {
        unsigned char *from=i*rsaSize+reinterpret_cast<unsigned char*>(data.data());
        unsigned char *to=new unsigned char[rsaSize];

        int len=RSA_public_decrypt(rsaSize,from,to,this->data->rsa,RSA_PKCS1_PADDING);

        result.append(reinterpret_cast<const char*>(to),len);
        delete [] to;
    }

    return result;
}

/*!
    从PEM格式创建实例.

    \sa toPEM()
*/

RSAPublicKey RSAPublicKey::fromPEM(QByteArray pem)
{
    BIO *bio=BIO_new(BIO_s_mem());

    BIO_write(bio,pem.constData(),pem.size());

    ::RSA *rsa=PEM_read_bio_RSAPublicKey(bio,NULL,NULL,NULL);

    ::RSA *pubRsa=RSAPublicKey_dup(rsa);

    RSAKeyPrivate *pKey=new RSAKeyPrivate;
    pKey->rsa=pubRsa;

    BIO_free(bio);
    RSA_free(rsa);

    return RSAPublicKey(pKey);
}

/*!
    \class RSAKeyMaker
    \brief RSA密钥对生成器类

    该类只有一个静态函数 makeKeyPair() 用于生成密钥对.
*/

/*!
    生成密钥对.

    \a bits 为密钥长度，单位比特； \a publicExp 指定公钥指数.
*/

QPair<RSAPrivateKey,RSAPublicKey> RSAKeyMaker::makeKeyPair(int bits,RSAKey::PublicExp publicExp)
{
    ::RSA *rsa=RSA_generate_key(bits,getPublicExp(publicExp),NULL,NULL);
    ::RSA *publicRsa=RSAPublicKey_dup(rsa);
    ::RSA *privateRsa=RSAPrivateKey_dup(rsa);

    RSA_free(rsa);

    RSAKey::RSAKeyPrivate *ppublicRsa=new RSAKey::RSAKeyPrivate;
    ppublicRsa->rsa=publicRsa;
    RSAKey::RSAKeyPrivate *pprivateRsa=new RSAKey::RSAKeyPrivate;
    pprivateRsa->rsa=privateRsa;

    auto pair=qMakePair(RSAPrivateKey(pprivateRsa),RSAPublicKey(ppublicRsa));

    delete ppublicRsa;
    delete pprivateRsa;

    return pair;
}

/*
    获得 \c RSAKey::PublicExp 类型所代表的实际值.

    \sa RSAKey::PublicExp
*/

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
