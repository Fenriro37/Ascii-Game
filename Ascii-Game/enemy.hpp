#pragma once
#include "Cast.hpp"

class enemy: public cast{
protected:    
    int life;
    bool alive;
    bool direction;
    int fireDelay;

public:
    enemy();
    enemy(bool rigged);
    void setLife(int newLife);
    void decreaseLife();
    void setAlive();
    int  getLife();
    bool getAlive();
    bool getDirection();
    void setDirection();
    int getFireDelay();
    void increaseFireDelay();
    void resetFireDelay();

};