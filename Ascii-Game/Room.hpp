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
    bulletNode* next = NULL;
    //bulletNode* prev = NULL;
};

struct control{
    bool available;
    bool thereIsMonster;
    int numberOfMonsters;
};

class room {
protected:
    int roomNum = 1;
    control freeRow[roomHeight];
    char view[roomHeight*roomWidth];

    itemNode* currentBonus;
    enemyNode* currentMonsters;
    bulletNode* currentAmmo;
       
public:
       // da capire come rendere protected
    room();
    room(int lvl);

    void setRoomNum(int newRoomNum);
    int getRoomNum();
    itemNode* getCurrentBonus();
    enemyNode* getCurrentMonsters();
    bulletNode* getCurrentAmmo();
    char* getView();

    void initializeView();
    void thirdCase(int row, int holes, int position, int maxHoles); 
    void drillRow(const int currentLevel); //funzione per interagire con platforms
    int findYinRow(int row, int second);
    void roomGenerator();
    int sizeofArray(char charArray[]);

    void initializeItems(int currentLevel);
    void initializeEnemies(int currentLevel);
    /*Funzione per controllare che la cella sia vuota*/
    bool checkRow(int row, cast character);
    bool isAvailable(int x, int y, cast rookie);
    void spawnItems();
    void spawnEnemies();

    enemyNode* findMoster(int x, int y);
    itemNode* findBonus(int x, int y);
    bulletNode* findAmmo(int x, int y);
    /*Funzione per riposizionare il personaggio all'inizio di ogni livello*/
    void nextLevelPos();
    /*Funzione per riposizionare il personaggio in caso di ritorno al livello precedente*/
    void prevLevelPos();

    //Cast IA
    void addToList(bulletNode* newNode);
    void deleteNodes();
    void generateBullet(bool direction, cast shooter);
    bool bulletCollision(int x, int y);
    void bulletMove();
    bool enemyCollision(enemyNode* currentEnemy);
    void enemyMove();


};