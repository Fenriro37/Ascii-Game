#pragma once
#include "Cast.hpp"
#include "Constants.hpp"

class bullet: public cast{
protected:
    bool direction;
    bool alive;

public:
    bullet();
    bullet(bool newDirection);
    bool getDirection();
    void setDirection();
    void setAliveFalse();
    bool getAlive();
};