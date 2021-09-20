#pragma once
#include "Cast.hpp"

class enemy: public cast{
protected:    
    int life;
    bool alive;
    bool direction;

public:
    enemy();
    void setLife(int newLife);
    void setAlive();
    int  getLife();
    bool getAlive();
    bool getDirection();
    void setDirection();

    //IA and types

};

