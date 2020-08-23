#include "targetslotpainter.h"
#include "qgraphicsitem.h"
targetSlotPainter::targetSlotPainter()
{

}

//new
void targetSlotPainter::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget,
                            QRectF slot)
{

    QBrush brush(Qt::lightGray);
    painter->fillRect(slot, brush);
    painter->drawRect(slot);
    painter->setBrush(Qt::green);
    painter->drawRect(0, 0, 5000, 250);
    painter->drawRect(0, 2250, 5000, 250);

}
