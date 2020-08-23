#include "garage.h"
#include "QDebug"
Garage::Garage()
{
    QGraphicsScene();
}
Garage::Garage(qreal x, qreal y, qreal width, qreal height)
{
QGraphicsScene(x, y, width, height);
}
bool Garage::outOfBound(QPointF point){
    return point.x() >= this->width() || point.x() < 0 || point.y() >= this->height() || point.y() < 0;
}

void Garage::drawBackground(QPainter * painter, const QRectF & rect)
{

    QPen pen(Qt::white);
    pen.setWidth(10);
    qreal scale = 500;


    qreal length = this->width(), width = this->height();
    painter->fillRect(this->sceneRect(), Qt::darkGray);
    painter->setPen(pen);
    for(int i = 0; i < length / scale; i++){
        painter->drawLine(i*scale, 0, i * scale,width);
    }
    for(int i = 0; i < width / scale; i++){
        painter->drawLine(0, i * scale, length, i*scale);
    }

}
