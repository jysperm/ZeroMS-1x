#ifndef OCONFIG_H
#define OCONFIG_H

#include "OCore_global.h"
#include <QtCore/QString>

OMS_NAMESPACE_BEGIN

class OCORESHARED_EXPORT Config
{
public:
    Config();

    QString get(QString key);

    QString operator [](QString key);
};

OMS_NAMESPACE_END


#endif // OCONFIG_H
