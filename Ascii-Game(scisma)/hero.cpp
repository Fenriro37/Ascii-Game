#include "hero.hpp"
#include "Variables.hpp"

hero::hero(){
    cast::setRowPos(startRowPos);
    cast::setColPos(startColPos);
    cast::setFigure(HERO);
    setLife(3);
    setBullet(200);
    delay = false;
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
    bullet -= 1;
}

void hero::setBullet(int newBullet){
    bullet = newBullet;
}

void hero::setLife(int newLife){
    life = newLife;
}

void hero::changeDelay(){
    delay = !delay;
}
void hero::setDelay(bool newDelay){
    delay = newDelay;
}

bool hero::getDelay(){
    return delay;
}