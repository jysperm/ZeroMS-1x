#ifndef GLOBAL_H
#define GLOBAL_H

#include "../public/OGlobal.h"
class OSettings;
class OServerCore;

const int VERNUM = 2;
const QString VERSION = QObject::trUtf8("2.1.0.%1").arg(::VERNUM);
const QString SERVER = QObject::trUtf8("ZeroMS-Server");
const QString PROTOCOL = QObject::trUtf8("2.0");

extern OSettings *config;
extern OServerCore *core;

//检查一个用户名是否在线
//实现在OServerCore.h
inline bool OIsOnline(QString uname);

#endif // GLOBAL_H
