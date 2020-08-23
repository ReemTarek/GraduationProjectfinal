#include "pavement.h"
#include <QtWidgets/QtWidgets>
#include<QBrush>
#include<QRectF>
Pavement::Pavement()
{
    setFlag(ItemIsMovable);
}
QRectF Pavement::boundingRect() const
{
    return QRectF(0, 0, 8000, 750);
}

void Pavement::setHorizontal()
{
    setRotation(0);
}

void Pavement::setVertical()
{
    setRotation(90);
}

void Pavement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    QRect pavement (0, 0, 8000, 750);
    QPixmap image("/home/zaynab/GP/Test2/Test/Test/Pavement.jpg");
    painter->drawPixmap(pavement, image);

}

void Pavement::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Right:
        setHorizontal();
        break;
    case Qt::Key_Left:
        setVertical();
        break;
    }
}
