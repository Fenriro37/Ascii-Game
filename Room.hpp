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
    void drillRow(const int currentLevel); //funzione per bucare  PLATFORMS
    void newHoles(int row, int position, int &holes, int start, int finish, bool alsoEqual);
    void oneCastCase(int row, int holes, int position, int maxHoles);
    void twoMonstersCase(int row, int holes, int position, int maxHoles);
    int findYinRow(int row, int second);
    bool checkRow(int row, cast character);
    //Funzione per controllare che la cella sia vuota
    bool isAvailable(int x, int y, cast rookie);
       
    void spawnItems();
    void spawnEnemies();
    void initializeItems(int currentLevel);
    void initializeEnemies(int currentLevel);

    void addToList(bulletNode* newNode);



/*       
    void setCurrentBonus(itemNode* newHead);
    void setCurrentMonster(enemyNode* newHead);
    void setCurrentAmmo(bulletNode* newHead);

    bulletNode* getCurrentAmmo();
*/
public:
    room();
    room(int lvl);
    
    itemNode* getCurrentBonus();
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
    //void bulletMove();
    //void enemyMove();
    bool enemyCollision(enemyNode* currentEnemy);
};