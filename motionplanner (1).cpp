#include "motionplanner.h"
#include "car.h"
#include "slot.h"
#include <qmath.h>
#include<QQueue>
#include<QPair>
#include<iostream>
#include<QTransform>
#include<QList>
#include<QDebug>
#define  Config QPair<QPointF, qreal>

using namespace std;

void MotionPlanner::getInitConfig(Garage *garage){
    foreach (QGraphicsItem *item, garage->items()) {
        if ( dynamic_cast<Car*>(item)) {

            initConfig.first = car->getCentre(item->rotation(), item->pos());
            initConfig.second = normalize(item->rotation());

        }

    }
}
void MotionPlanner::initializeVisited(){

    visited.resize(length);
    for(int i = 0; i < length; i++) {
        visited[i].resize(width);
        for(int j = 0; j < width; j++) {
            visited[i][j].resize(thetaLimit);
            visited[i][j].fill(0);
        }
    }


}



MotionPlanner::MotionPlanner(Garage *garage, Slot *target)
{
    this->garage = garage;
    this->target = target;
    this->car = new Car();
    this->bicycle = new Bicycle();
    getInitConfig(garage);



    targetConfig.first = target->getCentre();
    targetConfig.second = normalize(target->rotation());
    unitDistance = 500, unitDegree = 3, steerStep = 10;

    length = garage->width() / unitDistance;
    width = garage->height() / unitDistance;
    thetaLimit = 360 / unitDegree;

    initializeVisited();

    initializeParentState();



}

void MotionPlanner::initializeParentState(){

    parentState.resize(length);

    for(int i = 0; i < length; i++) {
        parentState[i].resize(width);
        for(int j = 0; j < width; j++)
            parentState[i][j].resize(thetaLimit);
    }

}
Config MotionPlanner::getNextState(Config curState, qreal velocity, qreal phi){

    Config nextState = bicycle->run(curState, velocity, phi);
    qreal x = curState.first.x(), y = curState.first.y(), theta = normalize(curState.second);
    int fx = x / unitDistance, fy = y / unitDistance, ftheta = theta / unitDegree;

    qreal nextX = nextState.first.x(), nextY = nextState.first.y(), nextTheta = normalize(nextState.second);

    if((int)(nextX / unitDistance) == fx && (int)(nextY / unitDistance) == fy && (int)(nextTheta / unitDegree) == ftheta)
        nextState = bicycle->run(nextState, velocity, phi);


    return nextState;

}
QPointF MotionPlanner::getCorner(QPointF pos, qreal rotation, QPointF corner) {
    QTransform *qtransform = new QTransform();
    qtransform->translate(pos.x(), pos.y());
    qtransform->rotate(rotation);
    return qtransform->map(corner);
}
bool MotionPlanner::isCarRectangleFree(Config state){

    qreal length = car->getLength(), width = car->getWidth();
    qreal rotation = state.second;
    QPointF topLeft = car->getTopLeftCorner(rotation, state.first);
    QPointF topRight = {length, 0};
    QPointF bottomLeft = {0,  width};
    QPointF bottomRight = {length, width};

    QVector<QPointF> carCorners({topLeft, topRight, bottomRight, bottomLeft});

    for(int i = 1; i < 4; i++){
        carCorners[i] = getCorner(topLeft, rotation, carCorners[i]);
    }
    for(int i = 0; i < 4; i++)
        if(garage->outOfBound(carCorners[i])){

            return 0;
        }


    QPolygonF carRect(carCorners);
    QList<QGraphicsItem*>intersectingItems = garage->items(carRect);

    for(auto item : intersectingItems)
        if((dynamic_cast <Car*>(item) == NULL) && item != target){
            qDebug()<<"intersection";
            return 0;
        }




    return 1;



}
bool MotionPlanner::valid(Config state){
    qreal x = state.first.x(), y = state.first.y(), theta = normalize(state.second);


    int fx = x / unitDistance, fy = y / unitDistance, ftheta = theta / unitDegree;

    return   fx>=0 && fy>=0 && ftheta>=0 && fx<length && fy<width && ftheta<thetaLimit && !visited[fx][fy][ftheta] && isCarRectangleFree(state);
}

bool MotionPlanner::targetIsReached(qreal x, qreal y, qreal theta){

    return hypot(x - targetConfig.first.x(), y - targetConfig.first.y()) <= 400&& fabs(theta - targetConfig.second) <= unitDegree;
}
QVector<Config> MotionPlanner::getPath(Config start, Config end){
   qreal x, y, theta;
   int fx, fy, ftheta;
   QVector<Config> path;

    while(end != start){

        x = end.first.x(), y = end.first.y(), theta = normalize(end.second);
        fx = x / unitDistance, fy = y / unitDistance, ftheta = theta / unitDegree;
        path.push_back(end);


        end = parentState[fx][fy][ftheta];

    }

    return path;

}
void MotionPlanner::addStateIfValid(Config curState, qreal phi, QQueue<Config>&states){

    Config nextState = getNextState(curState, 10, phi);


     if(valid(nextState)){

        int nextX = nextState.first.x() / unitDistance, nextY = nextState.first.y() / unitDistance;
        int nextTheta = normalize(nextState.second) / unitDegree;
        parentState[nextX][nextY][nextTheta] = curState;
        visited[nextX][nextY][nextTheta] = 1;
        states.enqueue(nextState);

     }
}
bool MotionPlanner::planPath(QVector<Config>&path){


    QQueue<Config>states;
    states.enqueue(initConfig);



  int phiMax = bicycle->getPhiMax();

    while(!states.empty()){

        Config head = states.dequeue();


        qreal x = head.first.x(), y = head.first.y(), theta = normalize(head.second);

        int fx = x / unitDistance, fy = y / unitDistance, ftheta = theta / unitDegree;

        if(targetIsReached(x, y, theta)) {

            path = getPath(initConfig, head);

            return 1;
        }

        visited[fx][fy][ftheta] = 1;



       for(qreal phi = 0; phi <= phiMax; phi += steerStep){
               addStateIfValid(head, phi, states);
               addStateIfValid(head, -phi, states);
        }




    }

    return 0;
}


qreal MotionPlanner::normalize(qreal angle){
    qreal value = angle + ceil( -angle / 360 ) * 360;
    if(value < 360 && value >= 0)
        return value;
    return normalize(value);
}


