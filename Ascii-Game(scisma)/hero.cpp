#include "hero.hpp"
#include "Variables.hpp"

hero::hero(){
    cast::setRowPos(startRowPos);
    cast::setColPos(startColPos);
    cast::setFigure(HERO);
    setLife(3);
    setBullet(9);
    setScore(50);
}

int hero::getLife(){
    return life;
}

int hero::getBullet(){
    return bullet;
}

void hero::decreaseLife(){
    if(life!=0)
        life -=1;
}

void hero::decreaseBullet(){
    bullet -= 1;
}

void hero::setBullet(int newBullet){
    bullet = newBullet;
}

void hero::setLife(int newLife){
    if(life+newLife > 0)
        life+=newLife;
    else 
        life = 0;
}

int hero::getScore(){
    return score;
}
void hero::setScore(int newScore){
    if(score+newScore > 0)
        score+=newScore;
    else 
        score = 0;
}
