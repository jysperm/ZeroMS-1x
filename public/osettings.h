#ifndef PUBLIC_OSETTINGS_H
#define PUBLIC_OSETTINGS_H

#include <QSettings>
#include "const.h"

class OSettings
{
public:
    OSettings(QString file=PUBLIC_CONFIG_FILE);
    virtual ~OSettings();
    inline QVariant operator[](QString key);
    void load();

    QSettings *config;
    QSettings *defaultConfig;
    QString configFile;
};

inline QVariant OSettings::operator[](QString key)
{
    return config->value(key,defaultConfig->value(key));
}

#endif // PUBLIC_OSETTINGS_H
