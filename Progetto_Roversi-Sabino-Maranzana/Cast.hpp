#pragma once
#ifndef CAST_H_INCLUDED
#define CAST_H_INCLUDED

#include <iostream>

using namespace std;

class cast{
    public: 
        int life;
        int damage;
        int experience;
        int XPos;
        int YPos;
    
    cast();

    cast(int hp, int dmg, int exp, int x, int y);

    void lvlUP();
    
    void hpchange(int val);

};


#endif