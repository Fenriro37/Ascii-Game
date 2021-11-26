#include "enemy.hpp"
#include "Variables.hpp"

enemy::enemy(){
    int life = 1;
    bool alive = false;
    //cast::figure = MONSTER;
    //direction = rand()%2;
    if(rand()%10 < 2){
        //torretta
        cast::figure = TURRET;
        fireDelay = 0;
    }
    else {
        cast::figure = MONSTER;
        direction = rand()%2;
    }
}

void enemy::setLife(int newLife){
    life = newLife;
}

void enemy::decreaseLife(){
    life--;
    if (life <= 0){
        alive = true;
    }
}

int  enemy::getLife(){
    return life;
}

void enemy::setAlive(){
    alive = true;
}

bool enemy::getAlive(){
    return alive;
}

bool enemy::getDirection(){
    return direction;
}

void enemy::setDirection(){
    direction = !direction;
}

int enemy::getFireDelay(){
    return fireDelay;
}

void enemy::increaseFireDelay(){
    fireDelay+=1;
}

void enemy::resetFireDelay(){
    fireDelay = 0;
}