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

        game();



};



#endif