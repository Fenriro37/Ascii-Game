#include "bullet.hpp"

bullet::bullet(bool newDirection){
    alive = false;  //parte a false e diventa true solo quando spariamo (premiamo j o k)
    direction = newDirection;
}
bool bullet::getDirection(){
    return direction;
}
bool bullet::getAlive(){
    return alive;
}
void bullet::setAlive(){
    alive = !alive;
}