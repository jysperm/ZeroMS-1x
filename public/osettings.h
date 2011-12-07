#ifndef PUBLIC_OSETTINGS_H
#define PUBLIC_OSETTINGS_H

#include <QSettings>
#include "const.h"

class OSettings
{
public:
    OSettings(QString file=PUBLIC_CONFIG_FILE);
    virtual ~OSettings();
    QVariant operator[](QString key);
    void load();

    QSettings *config;
    QSettings *defaultConfig;
    QString configFile;
};

#endif // PUBLIC_OSETTINGS_H
