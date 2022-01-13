#pragma once
#include "Cast.hpp"

class enemy: public cast{
protected:
    bool alive;
    bool direction;
    int fireDelay;

public:
    enemy();
    enemy(bool rigged);
    void setAliveFalse();
    bool getAlive();
    bool getDirection();
    void setDirection();
    int getFireDelay();
    void increaseFireDelay();
    void resetFireDelay();

};