#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED
#include "Cast.hpp"
#include "Variables.hpp"

class room{

    public:
    char view[height][width];
    cast monters[monsterNum];
    //array items 
    int roomNum;
    
    

    void setroomNum(int n){
        roomNum = n;
    }
    
    void encounter();

    void initializeView(); 

    void move(char mossa);

};


#endif