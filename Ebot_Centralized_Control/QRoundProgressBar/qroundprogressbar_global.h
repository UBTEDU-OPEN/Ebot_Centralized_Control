#ifndef QROUNDPROGRESSBAR_GLOBAL_H
#define QROUNDPROGRESSBAR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QROUNDPROGRESSBAR_LIBRARY)
#  define QROUNDPROGRESSBARSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QROUNDPROGRESSBARSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QROUNDPROGRESSBAR_GLOBAL_H