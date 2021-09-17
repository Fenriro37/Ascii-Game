#pragma once
#include "Cast.hpp"

class hero : public cast{
    protected: 
    int life;
    int bullet;
    int experience;

    public:
    hero();
    void decreaseLife();
    void setLife(int newLife);
    void setBullet(int newBullet);
    int getLife();
    int getBullet();
};