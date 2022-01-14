#pragma once

#include "hero.hpp"
#include "item.hpp"
#include "enemy.hpp"
#include "bullet.hpp"

struct itemNode {
    item Bonus;
    itemNode* next = NULL;
};

struct enemyNode{
    enemy monster;
    enemyNode* next = NULL;
};

struct bulletNode{
    bullet ammo;
    bulletNode* next = NULL;
};

// available: indica se una riga è libera
// thereIsMonster: una riga occupata da uno o due MONSTER
//      una riga può essere occupata da uno o due MONSTER o da una TURRET o da un BONUS
// numberOfMonsters: numero di mostri in una riga
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

    //utility
    void drillRow(); //funzione per bucare  PLATFORMS
    void newHoles(int row, int position, int &holes, int start, int finish, bool alsoEqual);
    void oneCastCase(int row);
    void twoMonstersCase(int row, int holes);
    int findYinRow(int row, int second);
    bool checkRow(int row, cast character);
    //Funzione per controllare che la cella sia vuota
    bool isAvailable(int x, int y, cast rookie);
       
    void spawnItems();
    void initializeItems();
    itemNode* getCurrentBonus();

    void spawnEnemies();
    void initializeEnemies();
    
    void addToList(bulletNode* newNode);

    void clearEnemies();
    void clearBonus();
    void clearBullet();


public:
    room();
    room(int lvl);
    
    
    bulletNode* getCurrentAmmo();
    enemyNode* getCurrentMonsters();

    void setRoomNum(int newRoomNum);
    int getRoomNum();

    char* getView();
    
    int toSingleArray(int x, int y);
    //Funzione per riposizionare il personaggio all'inizio di ogni livello
    void nextLevelPos();
    //Funzione per riposizionare il personaggio in caso di ritorno al livello precedente
    void prevLevelPos();
    
    void roomGenerator();

    enemyNode* findMoster(int x, int y);
    itemNode* findBonus(int x, int y);
    bulletNode* findAmmo(int x, int y);
    
    void generateBullet(bool direction, cast shooter);
    bool bulletCollision(int x, int y);
    bool enemyCollision(enemyNode* currentEnemy);

    void clearList();

};