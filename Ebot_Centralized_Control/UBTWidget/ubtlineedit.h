#ifndef UBTLINEEDIT_H
#define UBTLINEEDIT_H

#include "ubtwidget_global.h"

#include <QLineEdit>
#include <QColor>

class UBTDLGSHARED_EXPORT UBTLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    UBTLineEdit(QWidget* parent=0);
    UBTLineEdit(const QString &contents, QWidget* parent=0);
    ~UBTLineEdit();

    void setPlaceholderText(const QString &text);
    void setPlaceholderTextColor(const QColor &color);

    void paintEvent(QPaintEvent *e) override;

private:
    QString m_placeholderText;
    QColor m_placeholderTextColor;
};

#endif // UBTLINEEDIT_H
