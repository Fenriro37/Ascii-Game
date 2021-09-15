#include "enemy.hpp"

enemy::enemy(){
    int life = 1;
    bool alive = true;
    cast::figure = 'M';
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