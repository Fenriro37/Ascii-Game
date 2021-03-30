#include "Hero.hpp"

    hero::hero(int hp, int dmg, int exp, int x,int y){
        life = hp;
        damage = dmg;
        experience = exp;
        Xhero = x;
        Yhero = y;
    }; 

    void hero::lvlUP(){
    life++;
    damage++;
    }

    void hero::hpchange(int val){
    life += val;
    }

