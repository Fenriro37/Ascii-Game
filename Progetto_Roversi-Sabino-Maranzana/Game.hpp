#pragma once
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include "Room.hpp"

class game {
public:
    room currentroom;
    _CHAR_INFO* CIarray;
    HANDLE hStdout, hStdin, hNewScreenBuffer;
    _SMALL_RECT consoleSize;
    _COORD characterBufferSize, characterPosition;

    game();
    void generateroom();
    void stampView();
    void move();

    
};




#endif
