#ifndef UBTCUSTOMERCENTER_GLOBAL_H
#define UBTCUSTOMERCENTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UBTCUSTOMERCENTER_LIBRARY)
#  define UBTCUSTOMERCENTERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UBTCUSTOMERCENTERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UBTCUSTOMERCENTER_GLOBAL_H