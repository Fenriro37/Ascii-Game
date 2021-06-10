#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED
#include "windows.h"
#include "Cast.hpp"
#include "Variables.hpp"

class room{

    public:
    CHAR_INFO CIview[roomHeight*roomWidth];
    char view [roomHeight][roomWidth];
    cast monters[monsterNum];
    //array items 
    int roomNum;
    
    

    void setroomNum(int n){
        roomNum = n;
    }
    
    void encounter();

    void initializeView(); 

    void move(char mossa);

    void toCharInfo();

};

#endif