#include "OSettings.h"


OSettings::OSettings(QString file):config(0),defaultConfig(0)
{
    configFile=file;
    load();
}

OSettings::~OSettings()
{
    delete config;
    delete defaultConfig;
}

void OSettings::load()
{
    //如果没有配置文件,输出默认配置文件
    if(!QFile::exists(configFile))
    {
        QFile file(configFile);
        QFile defaultConfigFile(":/config.ini");
        defaultConfigFile.open(QFile::ReadOnly);
        file.open(QFile::WriteOnly);
        file.write(defaultConfigFile.readAll());
    }

    delete config;
    config = new QSettings(configFile, QSettings::IniFormat, qApp);
    delete defaultConfig;
    defaultConfig = new QSettings(":/config.ini", QSettings::IniFormat, qApp);
}

