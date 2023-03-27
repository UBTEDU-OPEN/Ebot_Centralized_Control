#ifndef UBTDLG_GLOBAL_H
#define UBTDLG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UBTWIDGET_LIBRARY)
#  define UBTDLGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UBTDLGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UBTDLG_GLOBAL_H
