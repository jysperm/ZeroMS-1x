#include <openssl/md5.h>
#include "Hash.h"

namespace ZeroMS {
namespace Base {
namespace Auth {

/*
    纯粹用于包装来自openssl的MD5_CTX
*/

class MD5::MD5Private
{
public:
    MD5_CTX md5;
};

/*!
    \class MD5
    \brief MD5封装

    MD5封装了MD5散列算法，可以通过成员函数 md5() 方便地对较短的数据进行散列.
    如果需要处理不连续的数据流，可创建该类的实例，并通过 append() 函数添加数据，最后通过 result() 函数取得结果.
*/

/*!
    构造一个类实例，可用于散列大量数据或不连续的数据流.
*/

MD5::MD5():isFinal(false)
{
    this->data=new MD5Private;
    MD5_Init(&this->data->md5);
}

/*!
    析构函数
*/

MD5::~MD5()
{
    if(!this->isFinal)
        this->result();
    delete this->data;
}

/*!
    将数据(\a data)添加到正在散列的数据流的末尾.
*/

void MD5::append(const QByteArray data)
{
    if(this->isFinal)
        this->clear();
    MD5_Update(&this->data->md5,
               reinterpret_cast<const unsigned char*>(data.constData()),
               data.size());
}

/*!
    获取之前所有数据的散列结果.
    调用该函数后，需要调用 \c clear() 函数重新初始化，以便进行下一次散列.

    该函数返回的是二进制形式的散列值.

    \sa clear()
*/

QByteArray MD5::result()
{
    unsigned char out[MD5_DIGEST_LENGTH];

    MD5_Final(out,&this->data->md5);
    this->isFinal=true;

    return QByteArray(reinterpret_cast<const char*>(out),MD5_DIGEST_LENGTH);
}

/*!
    清除当前进度，以便进行下一次全新的散列任务.
*/

void MD5::clear()
{
    if(!this->isFinal)
        this->result();
    MD5_Init(&this->data->md5);
    this->isFinal=false;
}

/*!
    \overload md5()

    该函数返回的是转换为十六进制字符串格式的散列值.
*/

QString MD5::md5(const QString data)
{
    unsigned char out[MD5_DIGEST_LENGTH];

    ::MD5(reinterpret_cast<const unsigned char*>(data.toUtf8().constData()),
        data.toUtf8().size(),
        out);

    return QByteArray(reinterpret_cast<const char*>(out),MD5_DIGEST_LENGTH).toHex();
}

/*!
    简写形式，适用于散列少量数据(\a data).

    该函数返回的是二进制形式的散列值.
*/

QByteArray MD5::md5(const QByteArray data)
{
    unsigned char out[MD5_DIGEST_LENGTH];

    ::MD5(reinterpret_cast<const unsigned char*>(data.constData()),
        data.size(),
        out);

    return QByteArray(reinterpret_cast<const char*>(out),MD5_DIGEST_LENGTH);
}

}}}   // namespace ZeroMS::Base::Auth
