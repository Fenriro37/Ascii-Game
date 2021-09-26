#pragma once
#include "Cast.hpp"

class hero : public cast{
protected: 
    int life;
    int bullet;
    bool delay;

public:

    hero();
    void decreaseLife();
    void setLife(int newLife);
    void setBullet(int newBullet);
    void decreaseBullet();
    int getLife();
    int getBullet();
    void setDelay();
    bool getDelay();
};