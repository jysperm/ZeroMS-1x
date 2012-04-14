#ifndef PUBLIC_OSETTINGS_H
#define PUBLIC_OSETTINGS_H

#include <QSettings>
#include "OGlobal.h"

class OSettings
{
    //资源类
    //该类是对QSettings的封装，用于从配置文件读取配置信息
public:
    explicit OSettings(QString file=PUBLIC_CONFIG_FILE);
    virtual ~OSettings();
    void load();//重新读取配置文件
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
