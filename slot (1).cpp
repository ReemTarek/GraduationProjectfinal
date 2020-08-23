#include "slot.h"
#include <QtWidgets/QtWidgets>
#include<QTransform>

Slot::Slot(SlotPainter *slotPainter)
{
    this->slotPainter = slotPainter;


    setFlag(ItemIsMovable);

}

Slot::~Slot()
{
    delete this->slotPainter;
}

QRectF Slot::boundingRect() const
{
    return QRectF(0, 0, 5000, 2500);
}

void Slot::rotateClockwise()
{
    setRotation(rotation() + 45);
}

void Slot::rotateAnticlockwise()
{
    setRotation(rotation() - 45);
}

void Slot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    slotPainter->paint(painter, option, widget, boundingRect());

}

void Slot::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Right:
        rotateClockwise();
        break;
    case Qt::Key_Left:
        rotateAnticlockwise();
        break;
    }
}

SlotPainter *Slot::getSlotPainter() const
{
    return slotPainter;
}

void Slot::setSlotPainter(SlotPainter *value)
{
    slotPainter = value;
}
QPointF Slot::getCentre(){
    double cx = boundingRect().width() / 5.0, cy = boundingRect().height() / 2.0;
    QPointF centre = {cx, cy};
    QTransform *qtransform = new QTransform();
    qtransform->translate(this->pos().x(), this->pos().y());
    qtransform->rotate(this->rotation());
    return qtransform->map(centre);
}
