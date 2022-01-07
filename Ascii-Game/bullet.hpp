#pragma once
#include "Cast.hpp"
#include "Variables.hpp"

class bullet: public cast{
protected:
    bool direction;
    bool alive;

public:
    bullet();
    bullet(bool newDirection);
    bool getDirection();
    bool getAlive();
    void setAlive();
    void setDirection();
};