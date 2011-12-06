#include <QCoreApplication>
#include <QSettings>
#include <QString>
#include <QFile>
#include "const.h"

QSettings *config=0;
QSettings *defaultConfig=0;

void loadConfig()
{
    //如果没有配置文件,输出默认配置文件
    if(!QFile::exists(CONFIG_FILE))
    {
        QFile file(CONFIG_FILE);
        QFile defaultConfigFile(":/config.ini");
        defaultConfigFile.open(QFile::ReadOnly);
        file.open(QFile::WriteOnly);
        file.write(defaultConfigFile.readAll());
    }
    if(config)
        DELETE(config);
    config=new QSettings(CONFIG_FILE,QSettings::IniFormat,qApp);
    if(!defaultConfig)
        defaultConfig=new QSettings(":/config.ini",QSettings::IniFormat,qApp);
}

QVariant configValue(QString key)
{
    return config->value(key,defaultConfig->value(key));
}
