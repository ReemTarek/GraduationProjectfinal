#include "freeslotpainter.h"
#include "qgraphicsitem.h"
FreeSlotPainter::FreeSlotPainter() {
    QGraphicsItem::ItemIsMovable;

}

//updated
void FreeSlotPainter::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget,
                            QRectF slot)
{

    QBrush brush(Qt::lightGray);
    painter->fillRect(slot, brush);
    painter->drawRect(slot);
    painter->setBrush(Qt::blue);
    painter->drawRect(0, 0, 5000, 250);
    painter->drawRect(0, 2250, 5000, 250);

}
