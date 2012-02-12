#ifndef PUBLIC_OSETTINGS_H
#define PUBLIC_OSETTINGS_H

#include <QSettings>
#include "OGlobal.h"

class OSettings
{
public:
    explicit OSettings(QString file=PUBLIC_CONFIG_FILE);
    virtual ~OSettings();
    void load();
    inline QVariant operator[](QString key);
protected:
    QSettings *config;
    QSettings *defaultConfig;
    QString configFile;
};

inline QVariant OSettings::operator[](QString key)
{
    return config->value(key,defaultConfig->value(key));
}

#endif // PUBLIC_OSETTINGS_H
