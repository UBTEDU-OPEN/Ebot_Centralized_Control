#ifndef UBTUTIL_GLOBAL_H
#define UBTUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UBTUTIL_LIBRARY)
#  define UBTUTILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UBTUTILSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UBTUTIL_GLOBAL_H
