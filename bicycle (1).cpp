#include "bicycle.h"
#include <QDebug>
#include<QTransform>

Bicycle::Bicycle()
{


}

QPair<QPointF, qreal> Bicycle::run(QPair<QPointF, qreal>curConfig, qreal velocity, qreal wheelsAngle)
{
    QPointF pos = curConfig.first;
    qreal rotation = curConfig.second;
    qreal theta = qDegreesToRadians(rotation);
    qreal dx = velocity * qCos(theta);
    qreal dy = velocity * qSin(theta);
    qreal phi = qDegreesToRadians(wheelsAngle);
    qreal dtheta = velocity / wheelBase * qTan(phi);
    qreal x = pos.x() + dx * dt * 1000;
    qreal y = pos.y() + dy * dt * 1000;
    theta += dtheta * dt;
    theta = qRadiansToDegrees(theta);

    return {{x,y}, theta};
}

QPair<qreal, qreal> Bicycle::run(QPair<QPointF, qreal> curConfig, QPair<QPointF, qreal> nxtConfig)
{
    qreal xDot = (nxtConfig.first.x() - curConfig.first.x()) / (1000 * dt);
    qreal yDot = (nxtConfig.first.y() - curConfig.first.y()) / (1000 * dt);
    qreal v = hypot(xDot, yDot);
    qreal nxtTheta = qDegreesToRadians(nxtConfig.second);
    qreal thetaDot = (nxtTheta - qDegreesToRadians(curConfig.second)) / dt;
    qreal phi = qAtan2(wheelBase * thetaDot, v);
    qreal wheelsAngle = qRadiansToDegrees(phi);

  /*  if (wheelsAngle > 35) {
        qDebug() << wheelsAngle << endl << "not steerable" << endl;
    }
*/
    return {v, wheelsAngle};
}

int Bicycle::getPhiMax() const
{

    return phiMax;
}


qreal Bicycle::getLength() const
{
    return length;
}

qreal Bicycle::getWidth() const
{
    return width;
}


