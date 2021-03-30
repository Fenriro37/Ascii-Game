#include "Enemy.hpp"
 
    enemy::enemy(int hp, int dmg, int exp, int x,int y){
            life = hp;
            damage = dmg;
            experience = exp;
            Xenemy = x;
            Yenemy = y;
        }         

    void enemy::lvlUP(){
        life++;
        damage++;
    }
    
    void enemy::hpchange(int val){
        life += val;
    }
