#include <openssl/sha.h>
#include "Hash.h"

namespace ZeroMS
{

namespace Auth
{

class SHA::SHAPrivate
{
public:
    virtual ~SHAPrivate(){}
};

class SHA::SHA1Private : public SHA::SHAPrivate
{
public:
    SHA_CTX data;
};

class SHA::SHA256Private : public SHA::SHAPrivate
{
public:
    SHA256_CTX data;
};

class SHA::SHA512Private : public SHA::SHAPrivate
{
public:
    SHA512_CTX data;
};

SHA::SHA(SHAType type):isFinal(false),type(type)
{
    switch(this->type)
    {
    case SHA1:
        this->data=new SHA1Private;
        SHA1_Init(&dynamic_cast<SHA1Private*>(this->data)->data);
        break;
    case SHA224:
        this->data=new SHA256Private;
        SHA224_Init(&dynamic_cast<SHA256Private*>(this->data)->data);
        break;
    case SHA256:
        this->data=new SHA256Private;
        SHA256_Init(&dynamic_cast<SHA256Private*>(this->data)->data);
        break;
    case SHA384:
        this->data=new SHA512Private;
        SHA384_Init(&dynamic_cast<SHA512Private*>(this->data)->data);
        break;
    case SHA512:
        this->data=new SHA512Private;
        SHA512_Init(&dynamic_cast<SHA512Private*>(this->data)->data);
        break;
    }
}

SHA::~SHA()
{
    if(!this->isFinal)
        this->result();
    delete this->data;
}

void SHA::append(const QByteArray data)
{
    if(this->isFinal)
        this->clear();

    const unsigned char* cucData=reinterpret_cast<const unsigned char*>(data.constData());

    switch(this->type)
    {
    case SHA1:
        SHA1_Update(&dynamic_cast<SHA1Private*>(this->data)->data,cucData,data.size());
        break;
    case SHA224:
        SHA224_Update(&dynamic_cast<SHA256Private*>(this->data)->data,cucData,data.size());
        break;
    case SHA256:
        SHA256_Update(&dynamic_cast<SHA256Private*>(this->data)->data,cucData,data.size());
        break;
    case SHA384:
        SHA384_Update(&dynamic_cast<SHA512Private*>(this->data)->data,cucData,data.size());
        break;
    case SHA512:
        SHA512_Update(&dynamic_cast<SHA512Private*>(this->data)->data,cucData,data.size());
        break;
    }
}

QByteArray SHA::result()
{
    int digestLength=SHA::digestLength(this->type);

    unsigned char *out=new unsigned char[digestLength];

    switch(this->type)
    {
    case SHA1:
        SHA_Final(out,&dynamic_cast<SHA1Private*>(this->data)->data);
        break;
    case SHA224:
        SHA224_Final(out,&dynamic_cast<SHA256Private*>(this->data)->data);
        break;
    case SHA256:
        SHA256_Final(out,&dynamic_cast<SHA256Private*>(this->data)->data);
        break;
    case SHA384:
        SHA384_Final(out,&dynamic_cast<SHA512Private*>(this->data)->data);
        break;
    case SHA512:
        SHA512_Final(out,&dynamic_cast<SHA512Private*>(this->data)->data);
        break;
    }

    this->isFinal=true;

    return QByteArray(reinterpret_cast<const char*>(out),digestLength);
}

void SHA::clear()
{
    if(!this->isFinal)
        this->result();

    switch(this->type)
    {
    case SHA1:
        SHA1_Init(&dynamic_cast<SHA1Private*>(this->data)->data);
        break;
    case SHA224:
        SHA224_Init(&dynamic_cast<SHA256Private*>(this->data)->data);
        break;
    case SHA256:
        SHA256_Init(&dynamic_cast<SHA256Private*>(this->data)->data);
        break;
    case SHA384:
        SHA384_Init(&dynamic_cast<SHA512Private*>(this->data)->data);
        break;
    case SHA512:
        SHA512_Init(&dynamic_cast<SHA512Private*>(this->data)->data);
        break;
    }

    this->isFinal=false;
}

QString SHA::sha(const QString data,SHAType type)
{
    int digestLength=SHA::digestLength(type);

    unsigned char *out=new unsigned char[digestLength];

    auto cucData=reinterpret_cast<const unsigned char*>(data.toUtf8().constData());

    switch(type)
    {
    case SHA1:
        ::SHA(cucData,data.toUtf8().size(),out);
        break;
    case SHA224:
        ::SHA224(cucData,data.toUtf8().size(),out);
        break;
    case SHA256:
        ::SHA256(cucData,data.toUtf8().size(),out);
        break;
    case SHA384:
        ::SHA384(cucData,data.toUtf8().size(),out);
        break;
    case SHA512:
        ::SHA512(cucData,data.toUtf8().size(),out);
        break;
    }

    return QByteArray(reinterpret_cast<const char*>(out),digestLength).toHex();
}

QByteArray SHA::sha(const QByteArray data,SHAType type)
{
    int digestLength=SHA::digestLength(type);

    unsigned char *out=new unsigned char[digestLength];

    auto cucData=reinterpret_cast<const unsigned char*>(data.constData());

    switch(type)
    {
    case SHA1:
        ::SHA(cucData,data.size(),out);
        break;
    case SHA224:
        ::SHA224(cucData,data.size(),out);
        break;
    case SHA256:
        ::SHA256(cucData,data.size(),out);
        break;
    case SHA384:
        ::SHA384(cucData,data.size(),out);
        break;
    case SHA512:
        ::SHA512(cucData,data.size(),out);
        break;
    }

    return QByteArray(reinterpret_cast<const char*>(out),digestLength);
}

int SHA::digestLength(SHAType type)
{
    switch(type)
    {
    case SHA1:
        return SHA_DIGEST_LENGTH;
    case SHA224:
        return SHA224_DIGEST_LENGTH;
    case SHA256:
        return SHA256_DIGEST_LENGTH;
    case SHA384:
        return SHA384_DIGEST_LENGTH;
    case SHA512:
        return SHA512_DIGEST_LENGTH;
    }
    return -1;
}

}   //namespace Auth

}   //namespace ZeroMS
