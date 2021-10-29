#include "hero.hpp"
#include "Variables.hpp"

hero::hero(){
    cast::setRowPos(startRowPos);
    cast::setColPos(startColPos);
    cast::setFigure(HERO);
    setLife(3);
    setBullet(99);
    setScore(50);
}

int hero::getLife(){
    return life;
}

int hero::getBullet(){
    return bullet;
}

int hero::getScore(){
    return score;
}

void hero::decreaseLife(){
    //if(life!=0)
        life -=1;
}

void hero::decreaseBullet(){
    //if(bullet!=0)
        bullet -= 1;
}

void hero::decreaseScore(int damage){
    if(score - damage > 0)
        score -= damage;
    else
        score = 0; 
}

void hero::setBullet(int newBullet){
    if(bullet + newBullet > maxAmmo)
        bullet = maxAmmo;
    else 
        bullet += newBullet;
}

void hero::setLife(int newLife){
    if(life + newLife > maxHp)
        life = maxHp;
    else 
        life += newLife;
}

void hero::setScore(int newScore){
    if(score + newScore > maxScore)
        score = maxScore;
    else 
        score += newScore;
}
