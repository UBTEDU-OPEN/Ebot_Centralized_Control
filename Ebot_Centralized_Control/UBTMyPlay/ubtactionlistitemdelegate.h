#ifndef UBTACTIONLISTITEMDELEGATE_H
#define UBTACTIONLISTITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>

class QPainter;
class QStyleOptionViewItem;
class QModelIndex;

class UBTActionListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    enum UBTActionListItemDataRole {
        PlayingFlagRole = Qt::UserRole,
        ActionDurationRole
    };

public:
    UBTActionListItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // UBTACTIONLISTITEMDELEGATE_H
