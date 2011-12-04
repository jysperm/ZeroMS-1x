#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCoreApplication>
#include <QSettings>
#include <QString>
#include <QFile>
#include "const.h"

namespace OMS
{

//配置文件路径
const QString CONFIG_FILE = QObject::trUtf8("./config.ini");

QSettings *config;

void loadConfig(QString file=CONFIG_FILE)
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

}      // namespace OMS

#endif // SETTINGS_H
