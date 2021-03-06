#include "bullet.hpp"

bullet::bullet(){
    cast::setFigure(BULLET);
    alive = true;
}

bullet::bullet(bool newDirection){
    cast::setFigure(BULLET);
    alive = true;
    direction = newDirection;
}

bool bullet::getDirection(){
    return direction;
}

bool bullet::getAlive(){
    return alive;
}

void bullet::setAliveFalse(){
    alive = false;
}

