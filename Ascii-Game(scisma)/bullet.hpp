#pragma once
#include "Cast.hpp"

class bullet: public cast{
protected:
    bool direction;
    bool alive;

public:
    bullet(bool newDirection);
    bool getDirection();
    bool getAlive();
    void setAlive();
};