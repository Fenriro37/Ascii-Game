#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include <iostream>

using namespace std;

class hero{
    public: 
        int life;
        int damage;
        int experience;
        int Xhero;
        int Yhero;
        //int firerate;
 
    hero(int hp, int dmg, int exp, int x,int y);

    void lvlUP();
    
    void hpchange(int val);

};

#endif