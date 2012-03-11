#ifndef QJSON_GLOBAL_H
#define QJSON_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QJSON_LIBRARY)
#  define QJSONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QJSONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QJSON_GLOBAL_H
