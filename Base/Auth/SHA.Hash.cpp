#include <openssl/sha.h>
#include "Hash.h"

namespace ZeroMS {
namespace Base {
namespace Auth {

/*
    纯粹用于包装来自openssl的SHA_CTX等结构

    SHA1使用SHA_CTX, SHA224和SHA256使用SHA256_CTX, SHA384和SHA512使用SHA512_CTX,
    参见头文件 <openssl/sha.h> .
*/

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

/*!
    \class SHA
    \brief SHA封装

    SHA封装了SHA系列散列算法，可以通过成员函数 sha() 方便地对较短的数据进行散列.
    如果需要处理不连续的数据流，可创建该类的实例，并通过 append() 函数添加数据，最后通过 result() 函数取得结果.
*/

/*!
    \enum SHA::SHAType

    该枚举指定了SHA系列算法中具体的一种.

    \value SHA1
           SHA1
    \value SHA224
           SHA224
    \value SHA256
           SHA256
    \value SHA384
           SHA384
    \value SHA512
           SHA512
*/

/*!
    构造一个类实例，可用于散列大量数据或不连续的数据流.

    \a type 指明要使用的具体算法.

    \sa SHA::SHAType
*/

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

/*!
    析构函数
*/

SHA::~SHA()
{
    if(!this->isFinal)
        this->result();
    delete this->data;
}

/*!
    将数据(\a data)添加到正在散列的数据流的末尾.
*/

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

/*!
    获取之前所有数据的散列结果.
    调用该函数后，需要调用\c clear() 函数重新初始化，以便进行下一次散列.

    该函数返回的是二进制形式的散列值.

    \sa clear()
*/

QByteArray SHA::result()
{
    int digestLength=SHA::digestLength(this->type);

    unsigned char *out=new unsigned char[digestLength];

    switch(this->type)
    {
    case SHA1:
        SHA1_Final(out,&dynamic_cast<SHA1Private*>(this->data)->data);
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

/*!
    清除当前进度，以便进行下一次全新的散列任务.
*/

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

/*!
    \overload sha()

    该函数返回的是转换为十六进制字符串格式的散列值.

    \a type 指明要使用的具体算法.

    \sa SHA::SHAType
*/

QString SHA::sha(const QString data,SHAType type)
{
    int digestLength=SHA::digestLength(type);

    unsigned char *out=new unsigned char[digestLength];

    auto cucData=reinterpret_cast<const unsigned char*>(data.toUtf8().constData());

    switch(type)
    {
    case SHA1:
        ::SHA1(cucData,data.toUtf8().size(),out);
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

/*!
    简写形式，适用于散列少量数据(\a data).

    该函数返回的是二进制形式的散列值.

    \a type 指明要使用的具体算法.

    \sa SHA::SHAType
*/

QByteArray SHA::sha(const QByteArray data,SHAType type)
{
    int digestLength=SHA::digestLength(type);

    unsigned char *out=new unsigned char[digestLength];

    auto cucData=reinterpret_cast<const unsigned char*>(data.constData());

    switch(type)
    {
    case SHA1:
        ::SHA1(cucData,data.size(),out);
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

/*!
    返回 \a type 指定的散列算法所产生的散列值的长度.
*/

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

}}}   // namespace ZeroMS::Base::Auth
