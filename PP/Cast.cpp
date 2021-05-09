 #include "Cast.hpp" 

    cast::cast(){ 
        int life=0;
        int damage=0;
        int experience=0;
        int XPos=0;
        int YPos=0;
    }
    
    cast::cast(int hp, int dmg, int exp, int x){
        life = hp;
        damage = dmg;
        experience = exp;
        position = x;
    } 

    void cast::lvlUP(){
        life++;
        damage++;
    }

    void cast::hpchange(int val){
        life += val;
    }
