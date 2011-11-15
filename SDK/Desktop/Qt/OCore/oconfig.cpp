#include "oconfig.h"

OMS_NAMESPACE_BEGIN

Config::Config()
{
}

QString Config::get(QString key)
{
    ////////////////////////
    // There is nothing.!!
    ////////////////////////
    return QString("There is nothing.");
}

QString Config::operator [](QString key)
{
    return get(key);
}

OMS_NAMESPACE_END
