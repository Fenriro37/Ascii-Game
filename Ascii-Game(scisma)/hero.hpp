#pragma once
#include "Cast.hpp"

class hero : public cast{
protected: 
    int life;
    int bullet;
    int score;
public:
    hero();
    void decreaseLife();
    void setLife(int newLife);
    void setBullet(int newBullet);
    void setScore(int newScore);
    void decreaseBullet();
    int getLife();
    int getBullet();
    int getScore();
};