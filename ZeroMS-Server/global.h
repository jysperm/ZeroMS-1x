#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include "../public/OSettings.h"
#include "../public/OGlobal.h"

const int VERNUM = 1;
const QString VERSION =QObject::trUtf8("2.0.0.%1").arg(VERNUM);
const QString SERVER =QObject::trUtf8("ZeroMS-Server");

extern OSettings *config;

#endif // GLOBAL_H
