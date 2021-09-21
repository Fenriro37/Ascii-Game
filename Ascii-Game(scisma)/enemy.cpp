#include "enemy.hpp"
#include "Variables.hpp"
enemy::enemy(){
    int life = 1;
    bool alive = true;
    cast::figure = 'M';
    direction = rand()%2;
}
void enemy::setLife(int newLife){
    life = newLife;
}
void enemy::setAlive(){
    alive = false;
}
int  enemy::getLife(){
    return life;
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