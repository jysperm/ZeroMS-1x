#include <openssl/md5.h>
#include "Hash.h"

QString ZeroMS::Auth::Hash::md5(const QString data)
{
    unsigned char out[20];

    MD5((unsigned char*)(data.toUtf8().constData()),
        data.toUtf8().size(),
        out);

    return QByteArray((const char*)out,20).toHex();
}
