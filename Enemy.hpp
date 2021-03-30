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
 
   enemy(int hp, int dmg, int exp, int x,int y);        

    void lvlUP();
    
    void hpchange(int val);
};

#endif