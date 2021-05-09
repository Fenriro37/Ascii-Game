#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include "Room.hpp"

struct list{
    room myRoom;
    list* next;
    list* prev;
};

class game{
public:
    list roomList;
    HANDLE hStdout, hStdin;
    _SMALL_RECT smallRect;
    _COORD coordBottomRight, coordTopLeft;

    game();
    void stampView(); 

};



#endif