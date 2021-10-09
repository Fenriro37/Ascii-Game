#include "Room.hpp"
#include <string>

room::room(){
    for (int i=0;i<roomHeight;i++){
        if(i%2 == 0)
            freeRow[i].available = false;
        else
            freeRow[i].available = true;
    }
    nextLevelPos();
    roomGenerator();
}

room::room(int lvl){
    roomNum = lvl;
    for (int i=0;i<roomHeight;i++){
        if(i%2 == 0)
            freeRow[i].available = false;
        else
            freeRow[i].available = true;
    }
    roomGenerator();
}

//Funzione per creare randomicamente una serie di piattaforme
void room::generateRow(int currentLevel){
    // Inizializzare l'array con PLATFORM per indicare che la piattaforma è usabile
    for(int i=0; i<roomWidth; i++){
        platforms[i] = PLATFORM;
    }
    
    //variabile per contare quanti buchi dovremo creare
    int holes = 0; 
    int position = 0;

    //Doppia condizione per evitare che si creino troppi buchi
    while(currentLevel != 0 && holes < 5){ 
        position = rand()%roomWidth;
        //primo buco
        if(platforms[position] == PLATFORM && position!=0 && position!=1 && position!=roomWidth-1 && position!=roomWidth-2){
            platforms[position] = BLANK;
            holes++;
        }
        if(position-1>1)
            platforms[position-1] = BLANK;
        if(position+1 < roomWidth-2)
            platforms[position+1] = BLANK;
    }
}

//funzione per creare una stanza in maniera casuale
void room::roomGenerator(){
    for (int row = 0; row < roomHeight; row++) {
        generateRow(getRoomNum());
        for (int col = 0; col < roomWidth; col++) {
            //caso tetto
            if (row == 0){ 
                view[roomWidth * row + col] = ROOF;
            }
            //pavimento
            else if(row == roomHeight-1){
                view[roomWidth * row + col] = FLOOR;
            }
            //caso Muro
            else if (col == 0 || col == roomWidth - 1){ 
            view[roomWidth * row + col] = WALL;
            }
            //caso piattaforme
            else if (row%2==0 && row!=0){ 
                view[roomWidth * row + col] = platforms[col];
            }
            else {
                view[roomWidth * row + col] = BLANK;
            }    
        }
    }
    //modifiche manuali per angoli e "porte"
    view[0] = TOPLEFT;
    view[roomWidth-1] = TOPRIGHT;
    view[roomWidth * (roomHeight-2)] = BLANK;
    view[roomWidth * (roomHeight-2) + roomWidth-1] = BLANK;
    //inserimento protagonista
    view[roomWidth * startRowPos + startColPos] = protagonist.getFigure();

    initializeItems(getRoomNum());
    initializeEnemies(getRoomNum());

    spawnItems();
    spawnEnemies();
}

//Funzione per inizializzare gli item della stanza corrente
//Quanti nemici contemporaneamente?1K
void room::initializeItems(int currentLevel){
    // O mettiamo un certo numero di bonus fissi per incentivare lo spostamento o ci affidiamo al caso
    int numOfBonus = rand()%3+1; //currentLevel / 3 + 1 || 
    int count = 1;
    item newItem;
    currentBonus = new itemNode();
    currentBonus->Bonus = newItem;
    currentBonus->next = NULL;
    while((numOfBonus-1)!=0 && count!=maxNumOfBonus){
        //inserimento in testa in una lista
        itemNode* tmp = new itemNode();
        item newItem;
        tmp->Bonus = newItem;
        tmp->next = currentBonus;
        currentBonus = tmp;
        count++; numOfBonus--;
    }
}

void room::initializeEnemies(int currentLevel){
    int numOfEnemies = rand()%3+1;
    int count = 1;
    enemy monster;
    currentMonsters = new enemyNode();
    currentMonsters->monster = monster;
    currentMonsters->next = NULL;
    while((numOfEnemies-1)!=0 && count!=maxNumOfEnemies){
        //inserimento in testa in una lista
        enemyNode* tmp = new enemyNode();
        enemy newEnemy;
        tmp->monster = newEnemy;
        tmp->next = currentMonsters;
        currentMonsters = tmp;
        count++; numOfEnemies--;
    }
}

bool room::isAvailable(int x, int y, cast rookie){
    /*Controlliamo che non sia in una riga pari per non sovrascrivere uno spazio bianco dedica 
    a un "buco" e che il nuovo elemento si trovi sopra una piattaforma
    Spawniamo prima i nemici e 
    */
    if(freeRow[x].available && view[roomWidth * x + y] == BLANK && view[roomWidth * (x+1) + y] != BLANK
        && roomWidth * x + y != roomWidth * (roomHeight-2)){  
        if(x == roomHeight-2 && y >= roomWidth/2 && y != roomWidth-1){
            freeRow[x].available = false;
        }
        else if(rookie.getFigure() != MONSTER && x != roomHeight-2 && !freeRow[x].thereIsMonster){
            freeRow[x].available = false;
        }
        else if(rookie.getFigure() == MONSTER && x != roomHeight-2 
                && view[roomWidth * (x+1) + y-1] == PLATFORM
                && view[roomWidth * (x+1) + y+1] == PLATFORM
                ){
            freeRow[x].thereIsMonster = true;
        }
        else 
            return false;
        return true;
    }
    return false;
}

void room::spawnItems(){
    int x, y;
    itemNode* iter = currentBonus;
    while(iter != NULL){
        //remind il booleano taken parte sempre come falso
        if(!iter->Bonus.getTaken()){
            x = rand()%roomHeight;
            y = rand()%roomWidth;
            if(isAvailable(x,y, iter->Bonus)){
                iter->Bonus.setRowPos(x);
                iter->Bonus.setColPos(y);
                view[roomWidth * x + y] = iter->Bonus.getFigure();
                iter = iter->next;
            }
        }
        else
            iter = iter->next;
    }
}

void room::spawnEnemies(){
    int x, y;
    enemyNode* iter = currentMonsters;
    while(iter != NULL){
        //remind i mostri partono sempre con il bool alive = true
        //torretta si prende una riga per sè
        if(!iter->monster.getAlive()){
            x = rand()%roomHeight;
            y = rand()%roomWidth;
            if(isAvailable(x,y,iter->monster)){
                iter->monster.setRowPos(x);
                iter->monster.setColPos(y);
                view[roomWidth * x + y] = iter->monster.getFigure();
                iter = iter->next; 
            }
        }
        else
            iter = iter->next;
    }
}

enemyNode* room::findMoster(int x, int y){
    enemyNode* iter = currentMonsters;
    while(iter != NULL){
        if(iter->monster.getRowPos() == x && iter->monster.getColPos() == y){
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

bool room::enemyCollision(enemyNode* currentEnemy){
    int offSet;
    offSet = (currentEnemy->monster.getDirection()==LEFT) ? -1 : 1;
    if(view[roomWidth * currentEnemy->monster.getRowPos() + (currentEnemy->monster.getColPos() + offSet)] == HERO){
        currentEnemy->monster.setAlive();
        protagonist.decreaseLife();
        return true;
    }
    else if(view[roomWidth * currentEnemy->monster.getRowPos() + (currentEnemy->monster.getColPos() + offSet)] == BULLET){
        bulletNode* currentAmmo = findAmmo(currentEnemy->monster.getRowPos(), currentEnemy->monster.getColPos() + offSet);
        currentEnemy->monster.setAlive();
        currentAmmo->ammo.setAlive();
        view[currentAmmo->ammo.getPos()] = BLANK;

        return true;
    }
    return false;
}

void room::enemyMove(){ //movimento orizzontale
    enemyNode* iter = currentMonsters;
    int offSet;
    while(iter != NULL){
        if(!iter->monster.getAlive()){
            //caso torretta
            if(iter->monster.getFigure() == TURRET){
                //caso sparo a sinistra
                if(protagonist.getColPos()<= iter->monster.getColPos() && iter->monster.getFireDelay() == fireRate){
                    generateBullet(LEFT, iter->monster);
                    iter->monster.resetFireDelay();
                }
                else if (iter->monster.getFireDelay()== fireRate){
                    generateBullet(RIGHT, iter->monster);
                    iter->monster.resetFireDelay();
                }
                else{
                    iter->monster.increaseFireDelay();
                }
            }
            //caso corridore
            else if(iter->monster.getFigure() == MONSTER){
                //if ternario : se condizione è vera -1, altrimenti 1
                offSet = (iter->monster.getDirection()==LEFT) ? -1 : 1;
                if(view[roomWidth * (iter->monster.getRowPos()+1) + (iter->monster.getColPos() + offSet)] != BLANK 
                    && view[roomWidth * iter->monster.getRowPos() + (iter->monster.getColPos() + offSet)] != WALL
                    && view[roomWidth * iter->monster.getRowPos() + (iter->monster.getColPos() + offSet)] != MONSTER
                    && roomWidth * iter->monster.getRowPos() + (iter->monster.getColPos() + offSet) != roomWidth * (roomHeight-2)
                    && roomWidth * iter->monster.getRowPos() + (iter->monster.getColPos() + offSet) != roomWidth * (roomHeight-2) + roomWidth-1){
                        if(enemyCollision(iter)){
                            view[roomWidth * iter->monster.getRowPos() + iter->monster.getColPos()] = BLANK;
                        }
                        else{
                            view[roomWidth * iter->monster.getRowPos() + iter->monster.getColPos()] = BLANK;
                            iter->monster.setColPos(iter->monster.getColPos() + offSet);
                            view[roomWidth * iter->monster.getRowPos() + iter->monster.getColPos()] = iter->monster.getFigure();
                        }
                }
                else {
                    iter->monster.setDirection();
                }
            }
        }
        iter = iter->next;
    }
}

itemNode* room::findBonus(int x, int y){
    itemNode* iter = currentBonus;
    while(iter != NULL){
        if(iter->Bonus.getRowPos() == x && iter->Bonus.getColPos() == y){
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

bulletNode* room::findAmmo(int x, int y){
    bulletNode* iter = currentAmmo;
    while(iter != NULL){
        if(iter->ammo.getRowPos() == x && iter->ammo.getColPos() == y){
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

void room::addToList(bulletNode* newNode){
    bulletNode* iter = currentAmmo;
    while (iter->next != NULL){
        iter = iter->next;    
    }
    iter->next = newNode;
    newNode->prev = iter;
}

void room::deleteNodes(){
    if(currentAmmo!= NULL){
        bulletNode* iter = currentAmmo;
        while(iter->next!=NULL){
            //delete nodo non in testa
            if(!iter->ammo.getAlive()){
                bulletNode* tmp = iter;
                if(iter->prev!=NULL){
                    iter->prev->next = iter->next;
                    iter->next->prev = iter->prev;
                }
                else{
                    iter->next->prev = NULL;
                }
                iter = iter->next;
                delete tmp;
                tmp = NULL; 
            }
            else{
                iter = iter->next;
            }
        }
        //delete nodo testa
        if (iter->prev == NULL && !iter->ammo.getAlive()){
            delete currentAmmo;
            currentAmmo = NULL;
            iter = NULL;
        }
        //delete nodo coda
        else if(!iter->ammo.getAlive()){
            iter->prev->next = NULL;
            delete iter;
            iter = NULL;
        }
    }
}

void room::generateBullet(bool direction, cast shooter){
    bullet newAmmo(direction);
    bulletNode* newBullet = new bulletNode();
    newBullet->ammo = newAmmo;
    newBullet->ammo.setColPos(shooter.getColPos());
    newBullet->ammo.setRowPos(shooter.getRowPos());
    if(currentAmmo == NULL){
        currentAmmo = newBullet;
    }
    else{
        addToList(newBullet);
    }
    int offSet = (direction == LEFT) ? -1 : 1;
    bulletCollision(newBullet->ammo.getRowPos(), newBullet->ammo.getColPos() + offSet);
}

bool room::bulletCollision(int x, int y){
    if(view[roomWidth * x + y] != BLANK){
        if(view[roomWidth * x + y] == MONSTER || view[roomWidth * x + y] == TURRET){
            enemyNode* foundMonster = findMoster(x, y);
            foundMonster->monster.setAlive();
            //caso nemico ucciso
            view[roomWidth * x + y] = BLANK;  
        }
        else if(view[roomWidth * x + y] == BULLET){
            bulletNode* foundAmmo = findAmmo(x, y);
            foundAmmo->ammo.setAlive();
            view[roomWidth * x + y] = BLANK;
        }
        else if(view[roomWidth * x + y] == HERO){
            protagonist.decreaseLife();
        }
        //i bonus se vengono colpiti sono distrutti
        else if(view[roomWidth * x + y] == HEART || view[roomWidth * x + y] == COIN || view[roomWidth * x + y] == MAGAZINE) {
            itemNode* foundBonus = findBonus(x, y);
            foundBonus->Bonus.setTaken();
            view[roomWidth * x + y] = BLANK;
        }
        return true;
    }
    return false;
}

void room::bulletMove(){
    int offSet;
    bulletNode* iter = currentAmmo;
    while(iter != NULL){
        if(!iter->ammo.getAlive()){
            offSet = (iter->ammo.getDirection() == LEFT) ? -1 : 1; 
            //caso muro sinistro, il proiettile sparisce e viene settato a false
            if(roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()-1 == roomWidth * iter->ammo.getRowPos()
                && offSet == -1){
                iter->ammo.setAlive();
                view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = BLANK;
            }
            //caso muro destro
            else if(roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()+1 == roomWidth * (iter->ammo.getRowPos()) + roomWidth-1
                && offSet == 1){
                iter->ammo.setAlive();
                view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = BLANK;
            }
            //caso collisione con nemici, bonus, hero e proiettili
            else if(bulletCollision(iter->ammo.getRowPos(), iter->ammo.getColPos() + offSet)){
                iter->ammo.setAlive();                  
                if (view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] != TURRET)
                    view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = BLANK;
            }
            //caso proiettile appena generato
            else if (view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] == HERO 
                     || view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] == TURRET){
                iter->ammo.setColPos(iter->ammo.getColPos() + offSet);
                view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = iter->ammo.getFigure();
            }
            //Proiettile si muove di uno
            else{
                view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = BLANK;
                iter->ammo.setColPos(iter->ammo.getColPos() + offSet);
                view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = iter->ammo.getFigure();
            }
        }
        iter = iter->next;
    }
    //deleteNodes();
}

void room::nextLevelPos(){
    protagonist.setRowPos(startRowPos);
    protagonist.setColPos(startColPos);
}

void room::prevLevelPos(){
    protagonist.setRowPos(endRowPos);
    protagonist.setColPos(endColPos);
}

void room::setRoomNum(int newRoomNum){
    roomNum = newRoomNum;
}

int room::getRoomNum(){
    return roomNum;
}

itemNode* room::getCurrentBonus(){
    return currentBonus;
}

enemyNode* room::getCurrentMonsters(){
    return currentMonsters;
}

bulletNode* room::getCurrentAmmo(){
    return currentAmmo;
}


char* room::getView(){
    return view;
}