#include <openssl/md5.h>
#include "Hash.h"

namespace ZeroMS {
namespace Base {
namespace Auth {

class MD5::MD5Private
{
public:
    MD5_CTX data;
};

MD5::MD5():isFinal(false)
{
    this->data=new MD5Private;
    MD5_Init(&this->data->data);
}

MD5::~MD5()
{
    if(!this->isFinal)
        this->result();
    delete this->data;
}

void MD5::append(const QByteArray data)
{
    if(this->isFinal)
        this->clear();
    MD5_Update(&this->data->data,
               reinterpret_cast<const unsigned char*>(data.constData()),
               data.size());
}

QByteArray MD5::result()
{
    unsigned char out[MD5_DIGEST_LENGTH];

    MD5_Final(out,&this->data->data);
    this->isFinal=true;

    return QByteArray(reinterpret_cast<const char*>(out),MD5_DIGEST_LENGTH);
}

void MD5::clear()
{
    if(!this->isFinal)
        this->result();
    MD5_Init(&this->data->data);
    this->isFinal=false;
}

QString MD5::md5(const QString data)
{
    unsigned char out[MD5_DIGEST_LENGTH];

    ::MD5(reinterpret_cast<const unsigned char*>(data.toUtf8().constData()),
        data.toUtf8().size(),
        out);

    return QByteArray(reinterpret_cast<const char*>(out),MD5_DIGEST_LENGTH).toHex();
}

QByteArray MD5::md5(const QByteArray data)
{
    unsigned char out[MD5_DIGEST_LENGTH];

    ::MD5(reinterpret_cast<const unsigned char*>(data.constData()),
        data.size(),
        out);

    return QByteArray(reinterpret_cast<const char*>(out),MD5_DIGEST_LENGTH);
}

}}}   // namespace ZeroMS::Base::Auth
