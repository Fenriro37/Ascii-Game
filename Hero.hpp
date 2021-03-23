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
 
        hero(int hp, int dmg, int exp, int x,int y){
            life = hp;
            damage = dmg;
            experience = exp;
            Xhero = x;
            Yhero = y;
        }        

    void lvlUP(){
        life++;
        damage++;
    }
    
    void hpchange(int val){
        life += val;
    }

};

#endif