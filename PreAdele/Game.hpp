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
    HANDLE hStdout, hStdin, hNewScreenBuffer;
    _SMALL_RECT consoleSize;
    _COORD characterBufferSize, characterPosition;

    game();
    void stampView(); 
};



#endif