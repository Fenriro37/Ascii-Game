#include "enemy.hpp"
#include "Variables.hpp"

enemy::enemy(){
    int life = 1;
    bool alive = true;
    if(rand()%10 <1){
        //torretta
        cast::figure = TURRET;
        fireDelay = 0;
    }
    else {
        cast::figure = MONSTER;
        direction = rand()%2;
    }
}

//costruttore truccato, sempre MONSTER
//idea: passare la probabilità al costruttore
enemy::enemy(bool rigged){
    int life = 1;
    bool alive = true;
    cast::figure = MONSTER;
    direction = rand()%2;
}

void enemy::setLife(int newLife){
    life = newLife;
}

int  enemy::getLife(){
    return life;
}

void enemy::decreaseLife(){
    life--;
    if (life <= 0){
        alive = false;
    }
}

void enemy::setAlive(){
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