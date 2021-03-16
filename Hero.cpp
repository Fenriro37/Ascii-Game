#include <iostream>

using namespace std;

class hero{
    private: 
        int life;
        int damage;
        int experience;
        //int firerate;
    public:
        hero(int hp, int dmg, int exp){
            life = hp;
            damage = dmg;
            experience = exp;
        }        

    void lvlUP(){
        life++;
        damage++;
    }
    
    void hpchange(int val){
        life += val;
    }

};
