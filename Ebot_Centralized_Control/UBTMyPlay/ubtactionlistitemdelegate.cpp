#include "ubtactionlistitemdelegate.h"

#include <qDebug>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QRect>
#include <QPixmap>

UBTActionListItemDelegate::UBTActionListItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void UBTActionListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto remainderRect = option.rect;
    remainderRect.adjust(20, 0, 0, 0);
    auto text = index.data(Qt::DisplayRole).toString();
    QRect paintRect = painter->boundingRect(remainderRect, Qt::AlignLeft | Qt::AlignVCenter, text);
    if (remainderRect.contains(paintRect)) { // paint name and playing icon
        painter->save();

        if (option.state & QStyle::State_Selected) {
            QBrush brush(QColor::fromRgb(228, 244, 249, 225));
            painter->fillRect(option.rect, brush);
        }

        auto isPlaying = index.data(PlayingFlagRole).toBool();
        if (isPlaying) {
            painter->setPen(QColor::fromRgb(0, 169, 233));
        }
        else {
            painter->setPen(QColor::fromRgb(41, 71, 101));
        }

        painter->drawText(paintRect, Qt::AlignLeft | Qt::AlignVCenter, text);   // paint name
        remainderRect.adjust(paintRect.width() + 10, 0, 0, 0);

        if (isPlaying) {       // paint playing icon
            paintRect = remainderRect;
            paintRect.setSize(QSize(10, 14));
            auto movY = (remainderRect.height() - 14) / 2;
            paintRect.translate(0, movY);
            if (remainderRect.contains(paintRect)) {
                auto playingPixmap = QPixmap(":/res/images/ic_inplay.png");
                painter->drawPixmap(paintRect, playingPixmap);
                remainderRect.adjust(paintRect.width(), 0, 0, 0);
            }
        }

        auto durationSec = index.data(ActionDurationRole).toInt(); // paint action duration
        auto durationMin = durationSec / 60;
        durationSec = durationSec % 60;
        text = QString("%1:%2").arg(durationMin).arg(durationSec, 2, 10, QChar('0'));
        paintRect = painter->boundingRect(option.rect.adjusted(0, 0, -20, 0), Qt::AlignRight | Qt::AlignVCenter, text);
        if (remainderRect.contains(paintRect)) {
            painter->setPen(QColor::fromRgb(178,185,192));
            painter->drawText(paintRect, Qt::AlignRight | Qt::AlignVCenter, text);
        }

        painter->restore();
    }
    else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

