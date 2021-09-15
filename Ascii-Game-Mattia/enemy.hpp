#pragma once
#include "Cast.hpp"

class enemy: public cast{
protected:    
    int life;
    bool alive;

public:
    enemy();
    void setLife(int newLife);
    void setAlive();
    int  getLife();
    bool getAlive();

    //IA and types
};

