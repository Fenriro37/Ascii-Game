#include "bullet.hpp"


bullet::bullet(){
    cast::setFigure(BULLET);
    alive = false;
}

bullet::bullet(bool newDirection){
    cast::setFigure(BULLET);
    alive = false;
    direction = newDirection;
}

bool bullet::getDirection(){
    return direction;
}

bool bullet::getAlive(){
    return alive;
}

void bullet::setAlive(){
    alive = true;
}

void bullet::setDirection(){
    direction = !direction;
}
