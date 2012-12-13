#ifndef ZEROMS_BASE_AUTH_HASH_H
#define ZEROMS_BASE_AUTH_HASH_H

#include <QString>

namespace ZeroMS
{

namespace Auth
{

class Hash
{
public:
    static QString md5(const QString data);
private:
    Hash(){};
};

}   //namespace Auth

}   //namespace ZeroMS

#endif // ZEROMS_BASE_AUTH_HASH_H
