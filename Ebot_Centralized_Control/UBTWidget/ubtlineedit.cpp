#include "ubtlineedit.h"

#include <QPainter>
#include <QPalette>
#include <QStyleOptionFrame>
#include <QRect>
#include <QStyle>
#include <QFontMetrics>
#include <QRect>
#include <QPoint>

UBTLineEdit::UBTLineEdit(QWidget* parent) :
    QLineEdit(parent)
{
    QPalette pal = palette();
    m_placeholderTextColor = pal.text().color();
    m_placeholderTextColor.setAlpha(128);
}

UBTLineEdit::UBTLineEdit(const QString &contents, QWidget *parent) :
    QLineEdit(contents, parent)
{
}

UBTLineEdit::~UBTLineEdit()
{
}

void UBTLineEdit::setPlaceholderText(const QString &text)
{
    m_placeholderText = text;
}

void UBTLineEdit::paintEvent(QPaintEvent *e)
{
    QLineEdit::paintEvent(e);
    if (!hasFocus() && text().isEmpty() && !m_placeholderText.isEmpty()) {
        QPainter p(this);
        p.setPen(m_placeholderTextColor);
        QFontMetrics fm = fontMetrics();
        int minLB = qMax(0, -fm.minLeftBearing());
        QRect lineRect = this->rect();
        QRect ph = lineRect.adjusted(minLB + 3, 0, 0, 0);
        QString elidedText = fm.elidedText(m_placeholderText, Qt::ElideRight, ph.width());
        p.drawText(ph, Qt::AlignVCenter, elidedText);
    }
}

void UBTLineEdit::setPlaceholderTextColor(const QColor &color)
{
    m_placeholderTextColor = color;
}

