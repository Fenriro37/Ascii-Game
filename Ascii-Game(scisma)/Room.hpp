#pragma once

#include "Variables.hpp"
#include "hero.hpp"
#include "item.hpp"
#include "enemy.hpp"
#include "bullet.hpp"

struct itemNode {
    item Bonus;
    itemNode* next;
};

struct enemyNode{
    enemy monster;
    enemyNode* next;
};

struct bulletNode{
    bullet ammo;
    bulletNode* next;
};

class room {
protected:
    int roomNum = 1;
    char platforms [roomWidth];
   
    itemNode* currentBonus;
    enemyNode* currentMonsters;
    bulletNode* currentAmmo = NULL;
       
public:
       // da capire come rendere protected
    char view[roomHeight*roomWidth];
    room();

    void setRoomNum(int newRoomNum);
    int getRoomNum();
    itemNode* getCurrentBonus();
    enemyNode* getCurrentMonsters();
    bulletNode* getCurrentAmmo();
    char* getView();

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

    enemyNode* findMoster(int x, int y);
    itemNode* findBonus(int x, int y);
    /*Funzione per riposizionare il personaggio all'inizio di ogni livello*/
    void nextLevelPos();
    /*Funzione per riposizionare il personaggio in caso di ritorno al livello precedente*/
    void prevLevelPos();

    //Cast IA
    void generateBullet(bool direction);
    bool bulletCollision(int x, int y);
    void bulletMove();
    void enemyMove();
};