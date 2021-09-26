#pragma once
#include "windows.h"
#include "Cast.hpp"
#include "hero.hpp"
#include "time.h" 
#include "stdlib.h"

#define LEFT 0
#define RIGHT 1
#define TOPLEFT (char)201
#define TOPRIGHT (char)187
#define BLANK ' '
#define ROOF (char)205
#define FLOOR (char)205
#define WALL (char)186
#define PLATFORM (char)223
//#define DOOR (char)8
#define LEFTDOOR (char)8
#define RIGHTDOOR (char)10
#define HEART (char)3
#define COIN (char)184
#define HERO 'P'
#define MONSTER 'M'
#define MAGAZINE (char)169
#define BULLET (char)9

//roomWidth anche in cast.cpp
const int roomWidth = 40;
const int roomHeight = 17;
const int consoleWidth = 70;
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



