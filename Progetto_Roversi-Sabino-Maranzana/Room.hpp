#pragma once
#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED
#include "windows.h"
#include "Variables.hpp"


class room {

public:
    room* next = NULL;
    room* prev = NULL;
    CHAR_INFO CIview[consoleHeight * consoleWidth];
    char view[roomHeight][roomWidth];
    int roomNum = 1;

    void encounter();

    void initializeView();

    void toCharInfo();

};

#endif
