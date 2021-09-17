#pragma once

#include "Room.hpp"

class game {
protected:
    
    _CHAR_INFO* CIarray;
    HANDLE hStdout, hStdin, hNewScreenBuffer;
    _SMALL_RECT consoleSize;
    _COORD characterBufferSize, characterPosition;
    int score;
///////////////////////////SET E GET
public:
    room* currentroom;
    


    game();
    void logic();
    void nextRoom();
    void prevRoom();
    void stampView();
    void move(char input);
    
    int getScore();
    void setScore(int newScore);

    void checkCollision(int row, int col);
    int findItem(int row, int col);
};
