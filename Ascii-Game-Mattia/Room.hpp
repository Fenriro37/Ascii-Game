#pragma once

#include "Variables.hpp"
#include "hero.hpp"
#include "item.hpp"
#include "enemy.hpp"

struct itemNode {
    item Bonus;
    itemNode* next;
};

struct enemyNode{
    enemy monster;
    enemyNode* next;
};

class room {

public:
    int roomNum = 1;
    char view[roomHeight][roomWidth];
    char platforms [roomWidth];
   
    itemNode* currentBonus;
    enemyNode* currentMonsters;
    room* next = NULL;
    room* prev = NULL;
    CHAR_INFO CIview[consoleHeight * consoleWidth];

    /////////////////////////////////////////////////////////////////////////
    //TODO: modificare i parametri come protetti aggiungendo setter e getter/
    /////////////////////////////////////////////////////////////////////////

    room();

    void setRoomNum(int newRoomNum);
    void setView(char newView[roomHeight][roomWidth]);
    int getRoomNum();
    void initializeView();
    void generateRow(int currentLevel); //funzione per interagire con platforms
    void roomGenerator();
    int sizeofArray(char charArray[]);
    void paste(char arrayToPaste[], int &count, int &col, int size);
    void toCharInfo();

    void initializeItems(int currentLevel);
    void initializeEnemies(int currentLevel);
    /*Funzione per controllare che la cella sia vuota*/
    bool isEmpty(int x, int y);
    void spawnItems();
    void spawnEnemies();
    /*Funzione per riposizionare il personaggio all'inizio di ogni livello*/
    void nextLevelPos();
    /*Funzione per riposizionare il personaggio in caso di ritorno al livello precedente*/
    void prevLevelPos();

    //Cast IA
    void enemyMove();
};