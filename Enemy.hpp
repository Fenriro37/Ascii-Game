#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

class enemy{
    public: 
        int life;
        int damage;
        int experience;
        int Xenemy;
        int Yenemy;
        //int firerate;
 
   enemy(int hp, int dmg, int exp, int x,int y){
            life = hp;
            damage = dmg;
            experience = exp;
            Xenemy = x;
            Yenemy = y;
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