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
protected:
    int roomNum = 1;
    char platforms [roomWidth];
   
    itemNode* currentBonus;
    enemyNode* currentMonsters;

public:
    char view[roomHeight][roomWidth];   // da capire come rendere protected
    
    room();

    void setRoomNum(int newRoomNum);
    int getRoomNum();
    itemNode* getCurrentBonus();
    enemyNode* getCurrentMonsters();
    room* getNext();
    room* getPrev();


    void setView(char newView[roomHeight][roomWidth]);
    void initializeView();
    void generateRow(int currentLevel); //funzione per interagire con platforms
    void roomGenerator();
    int sizeofArray(char charArray[]);

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