#pragma once

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

    int score;
    roomList* currentroom;

public:

    game();
    void logic();
    void nextRoom();
    void prevRoom();
    void stampView();
    void changeCellOfView(int position, char figure);
    bool checkNear(int row, int col, char figure);
    void move(char input);    
    void toCharInfo();
    void paste(char arrayToPaste[], int size, int &count, int &col);

    
    int getScore();
    void setScore(int newScore);

    void playerCollision(int row, int col);
    int findItem(int row, int col);
};