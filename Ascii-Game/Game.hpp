#pragma once
#include "windows.h"
#include "conio.h"
#include "string.h"

#include "Room.hpp"

struct roomList {
    room myRoom;
    roomList* next = NULL;
    roomList* prev = NULL;

    roomList(int lvl){
        myRoom = room(lvl);
    }
};

class game {
protected:
    
    _CHAR_INFO* CIarray;
    HANDLE hStdout, hStdin, hNewScreenBuffer;
    _SMALL_RECT consoleSize;
    _COORD characterBufferSize, characterPosition;
    CHAR_INFO CIview[consoleHeight * consoleWidth];

    roomList* currentroom;

    void title();
    void move(char input);  
    void nextRoom();
    void prevRoom();
    void stampView();
    void changeCellOfView(int position, char figure);
    bool checkFigure(int row, int col, char figure);
    void toCharInfo();
    void paste(char arrayToPaste[], int size, int &count, int &col);

    void playerCollision(int row, int col, int cameFromAbove);
    void bulletMove();
    void enemyMove();
    void gameOver();

public:
    game();
    void logic();
};