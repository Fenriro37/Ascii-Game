#pragma once
#include "windows.h"
#include "hero.hpp"
#include "time.h" 
#include "stdlib.h"

//DIREZIONE MOVIMENTO
#define LEFT 0
#define RIGHT 1

//MAPPATURA CARATTERI ROOM
#define TOPLEFT (char)201
#define TOPRIGHT (char)187
#define BOTTOMLEFT (char)200
#define BLANK ' '
#define ROOF (char)205
#define FLOOR (char)205
#define WALL (char)186
#define PLATFORM (char)223
#define LEFTDOOR (char)8
#define RIGHTDOOR (char)10
#define HEART (char)3
#define COIN (char)184
#define MAGAZINE (char)169
#define HERO 'P'
#define MONSTER 'M'
#define TURRET 'T'
#define BULLET (char)9

//MISURE
const int roomWidth = 40;
const int roomHeight = 13;
const int consoleWidth = roomWidth+30;
const int consoleHeight = roomHeight;

//COSTANTI DI GIOCO
const int maxNumOfBonus = 2;
const int startRowPos = roomHeight-2;
const int startColPos = 1;
const int endRowPos = roomHeight-2;
const int endColPos = roomWidth-2;
const int fireRate = 30;
const int maxHp = 3;
const int maxScore = 999999;
const int maxAmmo = 90;

//COSTANTI PER FORMULE
const int BULLET_DMG_MULT = 3;
const int COLLISION_DMG_MULT = 4;
const int BONUS_KILL_MULT = 2;
const int COIN_MULT = 8;
const int BULLET_BONUS = 4;

//Qui partono le variabili contenute in Variables.cpp
//Colori
extern WORD DEF_COLORFOREGROUND;
extern hero protagonist;



