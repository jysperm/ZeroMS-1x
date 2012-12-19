#ifndef ZEROMS_BASE_AUTH_HASH_H
#define ZEROMS_BASE_AUTH_HASH_H

#include <QString>

namespace ZeroMS {
namespace Base {
namespace Auth {

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

}}}   // namespace ZeroMS::Base::Auth

#ifdef QT_TESTLIB_LIB

#include <QObject>

namespace ZeroMS {
namespace UnitTest {
namespace Base {
namespace Auth {

class HashTest : public QObject
{
    Q_OBJECT
public:
    HashTest();
private slots:
    void testMD5_md5();
    void testMD5_MD5();
    void testSHA_sha();
    void testSHA_SHA();

    void testMD5_md5_data();
    void testSHA_sha_data();
};

}}}}    // namespace ZeroMS::UnitTest::Base::Auth

#endif

#endif // ZEROMS_BASE_AUTH_HASH_H
