#ifndef ZEROMS_BASE_AUTH_HASH_H
#define ZEROMS_BASE_AUTH_HASH_H

#include <QString>

namespace ZeroMS
{

namespace Auth
{

#ifndef INPRIVATE
class MD5Private;
#endif

class MD5
{
public:
    MD5();
    ~MD5();

    void append(const QByteArray data);
    QByteArray result();
    void clear();

    static QString md5(const QString data);
    static QByteArray md5(const QByteArray data);
private:
    bool isFinal;
    MD5Private *data;
};

}   //namespace Auth

}   //namespace ZeroMS

#endif // ZEROMS_BASE_AUTH_HASH_H
