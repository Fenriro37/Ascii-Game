#pragma once
#include "Cast.hpp"
#include "Constants.hpp"

class hero : public cast{
protected: 
    int life;
    int bullet;
    int score;
public:
    hero();
    
    void decreaseLife();
    void decreaseBullet();
    void decreaseScore(int damage);
    void increaseLife();
    void setBullet(int newBullet);
    void setScore(int newScore);
    int getLife();
    int getBullet();
    int getScore();
};