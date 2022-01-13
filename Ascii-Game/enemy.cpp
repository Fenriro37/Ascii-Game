#include "enemy.hpp"
#include "Variables.hpp"

enemy::enemy(){
    alive = true;
    if(rand()%10 <4){
        cast::figure = TURRET;
    }
    else {
        cast::figure = MONSTER;
    }
    fireDelay = 0;
    direction = rand()%2;
}

enemy::enemy(bool rigged){
    alive = rigged;
    cast::figure = MONSTER;
    fireDelay = 0;
    direction = rand()%2;
}

void enemy::setAliveFalse(){
    alive = false;
}

bool enemy::getAlive(){
    return alive;
}

void enemy::setDirection(){
    direction = !direction;
}

bool enemy::getDirection(){
    return direction;
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