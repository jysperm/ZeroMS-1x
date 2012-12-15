#ifndef ZEROMS_BASE_AUTH_HASH_H
#define ZEROMS_BASE_AUTH_HASH_H

#include <QString>

namespace ZeroMS
{

namespace Auth
{

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
    class MD5Private;

    bool isFinal;
    MD5Private *data;
};

class SHA
{
public:
    enum SHAType
    {
        SHA1,SHA224,SHA256,SHA384,SHA512
    };

    SHA(SHAType type=SHA1);
    ~SHA();

    void append(const QByteArray data);
    QByteArray result();
    void clear();

    static QString sha(const QString data,SHAType type=SHA1);
    static QByteArray sha(const QByteArray data,SHAType type=SHA1);

    static int digestLength(SHAType type);
private:
    class SHAPrivate;
    class SHA1Private;
    class SHA256Private;
    class SHA512Private;

    bool isFinal;
    SHAType type;
    SHAPrivate *data;
};

}   //namespace Auth

}   //namespace ZeroMS

#endif // ZEROMS_BASE_AUTH_HASH_H
