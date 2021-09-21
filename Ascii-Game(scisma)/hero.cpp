#include "hero.hpp"
#include "Variables.hpp"

hero::hero(){
    cast::setRowPos(startRowPos);
    cast::setColPos(startColPos);
    cast::setFigure('P');
    setLife(3);
    setBullet(2);
}

int hero::getLife(){
    return life;
}

int hero::getBullet(){
    return bullet;
}

void hero::decreaseLife(){
    life -=1;
}

void hero::decreaseBullet(){
    bullet--;
}

void hero::setBullet(int newBullet){
    bullet = newBullet;
}

void hero::setLife(int newLife){
    life = newLife;
}