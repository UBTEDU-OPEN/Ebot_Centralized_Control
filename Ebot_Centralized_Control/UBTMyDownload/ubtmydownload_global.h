#ifndef UBTMYDOWNLOAD_GLOBAL_H
#define UBTMYDOWNLOAD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UBTMYDOWNLOAD_LIBRARY)
#  define UBTMYDOWNLOADSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UBTMYDOWNLOADSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UBTMYDOWNLOAD_GLOBAL_H
