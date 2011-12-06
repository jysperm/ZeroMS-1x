#include <QCoreApplication>
#include <QSettings>
#include <QString>
#include <QFile>
#include "const.h"

QSettings *config;
QSettings *defaultConfig;

void loadConfig(QString file)
{
    //如果没有配置文件,输出默认配置文件
    if(!QFile::exists(CONFIG_FILE))
    {
        QFile file(CONFIG_FILE);
        QFile defaultConfig(":/config.ini");
        defaultConfig.open(QFile::ReadOnly);
        file.open(QFile::WriteOnly);
        file.write(defaultConfig.readAll());
    }
    if(config)
        DELETE(config);
    config=new QSettings(CONFIG_FILE,QSettings::IniFormat,qApp);
}

QVariant configValue(QString key)
{
    return config->value(key,defaultConfig->value(key));
}
