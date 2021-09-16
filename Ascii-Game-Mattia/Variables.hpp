#pragma once
#include "windows.h"
#include "Cast.hpp"
#include "hero.hpp"
#include "time.h" 
#include "stdlib.h"

const int roomWidth = 40;
const int roomHeight = 17;
const int consoleWidth = 80;
const int consoleHeight = roomHeight;

const int maxNumOfBonus = 5;
const int maxNumOfEnemies = 4;

const int startRowPos = roomHeight-2;
const int startColPos = 1;
const int endRowPos = roomHeight-2;
const int endColPos = roomWidth-2;

//Qui partono le variabili contenute in Variables.cpp
//Colori
extern WORD DEF_COLORFOREGROUND;

extern hero protagonist;



