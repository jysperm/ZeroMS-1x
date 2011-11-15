#ifndef OCORE_GLOBAL_H
#define OCORE_GLOBAL_H

#include <QtCore/qglobal.h>
#include "oglobal.h"

#if defined(OCORE_LIBRARY)
#  define OCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define OCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // OCORE_GLOBAL_H
