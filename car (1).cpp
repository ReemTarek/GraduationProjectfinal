#include "car.h"
#include <QtWidgets/QtWidgets>
#include<bicycle.h>
#include<QDebug>

QRectF Car::boundingRect() const
{
    return QRectF(0.0, 0.0, length, width);
}

void Car::setPath( QVector<QPair<QPointF, qreal>> path)
{
    this->path = path;
  //  path.push_back(path.back());
   // std::reverse(path.begin(), path.end());
}

void Car::run()
{
    //    QPair<qreal, qreal> state = bicycle.run({pos(), rotation()}, pos());
    //    velocity = state.first;
    //    wheelsAngle = state.second;

    if (!path.empty()) {
   //     qDebug() << pos() << ' ' << path.back() << endl;
        QPair<QPointF, qreal>curConfig = {getCentre(rotation(), pos()), rotation()};
        QPair<qreal, qreal> state = bicycle.run(curConfig,path.back());
        velocity = state.first;
        wheelsAngle = state.second;
     /*   velocity = path.back().first;
        wheelsAngle = path.back().second;*/
        path.pop_back();
     //    qDebug()<<"car r"<<rotation() ;
       //  qDebug()<<"bic r"<<bicycle.getRotation();
         //qDebug()<<"car pos"<<getCentre(rotation(), pos())<<" b pos"<<bicycle.getPos();

    } else {

        velocity = 0;
        wheelsAngle = 0;

    }
}

void Car::accelerate()
{
    if (velocity < 10)
        ++velocity;
}

void Car::decelerate()
{
    if (velocity > -10)
        --velocity;
}

void Car::turnLeft()
{
    if (wheelsAngle > -35)
        wheelsAngle -= 5;
}

void Car::turnRight()
{
    if (wheelsAngle < 35)
        wheelsAngle += 5;
}

void Car::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_I:
        accelerate();
        break;
    case Qt::Key_K:
        decelerate();
        break;
    case Qt::Key_L:
        turnRight();
        break;
    case Qt::Key_J:
        turnLeft();
        break;
    }
}

Car::Car()
    : velocity(0)
    , wheelsAngle(0)
{
    startTimer(1000/33);
    setPos(100, 100);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
    setZValue(1);
}

void Car::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF target(0.0, 0.0, length, width);
    QRectF source = target;
    QPixmap image("/home/zaynab/GP/Test2/Test/Test/Car.png");
    painter->drawPixmap(source, image, target);

}
QPointF Car::getCentre(double rotation, QPointF pos){

    double a = (length - wheelBase * 1000) / 2.0, b = width / 2.0;
    QPointF centre = {a, b};
    QTransform *qtransform = new QTransform();
    qtransform->translate(pos.x(), pos.y());
     qtransform->rotate(rotation);
    return qtransform->map(centre);
}
QPointF Car::getTopLeftCorner(double rotation, QPointF centre){

    double a = -(length - wheelBase * 1000) / 2.0, b = -width / 2.0;
    QPointF origin = {a, b};
    QTransform *qtransform = new QTransform();
    qtransform->translate(centre.x(), centre.y());
    qtransform->rotate(rotation);
    return qtransform->map(origin);
}
void Car::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    //    qDebug() << pos().x() << ' ' << pos().y() << endl;

    run();
    //    qDebug() << velocity << ' ' << wheelsAngle << endl;
    QPair<QPointF, qreal>curConfig = {getCentre(rotation(), pos()), rotation()};
   /* bicycle.setPos(getCentre(rotation(), pos()));
    bicycle.setRotation(rotation());
    bicycle.setWheelsAngle(wheelsAngle);
    bicycle.setVelocity(velocity);*/

    QPair<QPointF, qreal> config = bicycle.run(curConfig, velocity, wheelsAngle);
    setPos(getTopLeftCorner(config.second, config.first));
    setRotation(config.second);
    update();
}

qreal Car::getWheelBase() const
{
    return wheelBase;
}

qreal Car::getWidth() const
{
    return width;
}

qreal Car::getLength() const
{
    return length;
}
