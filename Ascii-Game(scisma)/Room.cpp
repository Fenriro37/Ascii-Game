#include "Room.hpp"
#include <string>

room::room(){
    roomNum = 1;
    roomGenerator();
    nextLevelPos();
}

//Funzione per creare randomicamente una serie di piattaforme
void room::generateRow(int currentLevel){
    // Inizializzare l'array con "-" per indicare che la piattaforma è usabile
    for(int i=0; i<roomWidth; i++)
        platforms[i] = (char)223;
    int holes = 0; //variabile per contare quanti buchi dovremo creare
    int random = 0;
    //Doppia condizione per evitare che si creino troppi buchi
    while(currentLevel != 0 && holes <= 10){ 
        random = rand()%roomWidth;
        if(platforms[random] == (char)223 ){
            platforms[random] = BLANK;
            holes++;
            currentLevel--;
        } 
    }
}

//funzione per creare una stanza in maniera casuale
void room::roomGenerator(){
    for (int row = 0; row < roomHeight; row++) {
        generateRow(getRoomNum());
        for (int col = 0; col < roomWidth; col++) {
            //caso tetto
            if (row == 0 && col != 0 && col != roomWidth-1){ 
                view[roomWidth * row + col] = ROOF;
            }
            else if(row == roomHeight-1){
                view[roomWidth * row + col] = FLOOR;
            }
            else if(row == 0 && col == 0){
                view[roomWidth * row + col] = TOPLEFT;
            }
            else if(row == 0 && col == roomWidth-1){
                view[roomWidth * row + col] = TOPRIGHT;
            }
            //caso Muro
            else if (col == 0 && row != roomHeight - 2 || col == roomWidth - 1 && row != roomHeight - 2){ 
            view[roomWidth * row + col] = WALL;
            }
            //caso Porte
            //else if (col == 0 && row == roomHeight - 2 || col == roomWidth - 1 && row == roomHeight - 2){ 
            //view[roomWidth * row + col] = DOOR;}
            /*generazione delle righe con le piattaforme che si troveranno solo sulle righe pari diverse da 0
            e dall'ultima
            Controllare 
            */
            else if ((row%2==0) && row!=0 && row != roomHeight - 1){ 
                view[roomWidth * row + col] = platforms[col];
            }
            else {
                view[roomWidth * row + col] = BLANK;
            }    
        }
    }
    view[roomWidth * (roomHeight-2)] = LEFTDOOR;
    view[roomWidth * (roomHeight-2) + roomWidth-1] = RIGHTDOOR;
    //inserimento protagonista
    view[roomWidth * startRowPos + startColPos] = protagonist.getFigure();

    /*  TODO: aggiornare initialize item con bonus casuali
          ora tutti i bonus sono copie 
    */

    initializeItems(getRoomNum());
    initializeEnemies(getRoomNum());

    spawnItems();
    spawnEnemies();
}

/*
* Doing sizeof myArray will get you the total number of bytes allocated for that array.
* You can then find out the number of elements in the array  by dividing
* by the size of one element in the array: sizeof myArray[0]
*/

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

//Funzione per inizializzare gli item della stanza corrente
//Quanti nemici contemporaneamente?
void room::initializeItems(int currentLevel){
    // O mettiamo un certo numero di bonus fissi per incentivare lo spostamento o ci affidiamo al caso
    int numOfBonus = rand()%4+1; //currentLevel / 3 + 1 || 
    int count = 1;

    currentBonus = new itemNode();
    item newItem;
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
    int numOfEnemies = rand()%4+1;
    int count = 1;
    enemy monster;
    currentMonsters = new enemyNode();
    currentMonsters->monster = monster;
    currentMonsters->next = NULL;
    while((numOfEnemies-1)!=0 && count!=maxNumOfEnemies){
        //inserimento in testa in una lista
        enemyNode* tmp = new enemyNode();
        tmp->monster = monster;
        tmp->next = currentMonsters;
        currentMonsters = tmp;
        count++; numOfEnemies--;
    }
}

bool room::isEmpty(int x, int y){
    /*Controlliamo che non sia in una riga pari per non sovrascrivere uno spazio bianco dedica 
    a un "buco" e che il nuovo elemento si trovi sopra una piattaforma
    */
    if(x%2 != 0 && view[roomWidth * x + y] == BLANK && view[roomWidth * (x+1) + y] != BLANK)   
        return true;
    return false;
}

/*Controllare il booleano per sapere se spawnare o no
  Aggiustare l'ultimo nodo che ora non viene stampato
*/
void room::spawnItems(){
    int x, y;
    itemNode* iter = currentBonus;
    while(iter != NULL){
        //remind il booleano taken parte sempre come falso
        if(!iter->Bonus.getTaken()){
            x = rand()%roomHeight;
            y = rand()%roomWidth;
            if(isEmpty(x,y)){
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
        //controllo da rivedere (perché !)
        if(iter->monster.getAlive()){
            x = rand()%roomHeight;
            y = rand()%roomWidth;
            if(isEmpty(x,y)){
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

void room::nextLevelPos(){
    protagonist.setRowPos(startRowPos);
    protagonist.setColPos(startColPos);
}

void room::prevLevelPos(){
    protagonist.setRowPos(endRowPos);
    protagonist.setColPos(endColPos);
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

void room::enemyMove(){ //movimento orizzontale
    enemyNode* iter = currentMonsters;
    while(iter != NULL){
        if(iter->monster.getAlive()){
            if(!iter->monster.getDirection()){
                //0 per movimento a sx
                if(view[roomWidth * (iter->monster.getRowPos()+1) + (iter->monster.getColPos()-1)] != BLANK 
                    && view[roomWidth * iter->monster.getRowPos() + (iter->monster.getColPos()-1)] != WALL
                        && view[roomWidth * iter->monster.getRowPos() + (iter->monster.getColPos()-1)] != MONSTER){
                            view[roomWidth * iter->monster.getRowPos() + iter->monster.getColPos()] = BLANK;
                            iter->monster.setColPos(iter->monster.getColPos()-1);
                            view[roomWidth * iter->monster.getRowPos() + iter->monster.getColPos()] = iter->monster.getFigure();
                }
                else {
                    iter->monster.setDirection();
                }
            }
            else {
                //1 per movimento a dx
                if(view[roomWidth * (iter->monster.getRowPos()+1) + (iter->monster.getColPos()+1)] != BLANK
                    && view[roomWidth * iter->monster.getRowPos() + (iter->monster.getColPos()+1)] != WALL
                        && view[roomWidth * iter->monster.getRowPos() + (iter->monster.getColPos()+1)] != MONSTER){
                            view[roomWidth * iter->monster.getRowPos() + iter->monster.getColPos()] = BLANK;
                            iter->monster.setColPos(iter->monster.getColPos()+1);
                            view[roomWidth * iter->monster.getRowPos() + iter->monster.getColPos()] = iter->monster.getFigure();
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

bool room::bulletCollision(int x, int y){
    if(view[roomWidth * x + y] != BLANK){
        if(view[roomWidth * x + y] == MONSTER){
            enemyNode* foundMonster = findMoster(x, y);
            foundMonster->monster.decreaseLife();
            //caso nemico ucciso
            if(foundMonster->monster.getLife() <= 0){
                view[roomWidth * x + y] = BLANK;
                foundMonster->monster.setAlive();
            }
        }
        //i bonus se vengono colpiti sono distrutti
        else {
            itemNode* foundBonus = findBonus(x, y);
            foundBonus->Bonus.setTaken();
            view[roomWidth * x + y] = BLANK;
        }
        return true;
    }
    return false;
}

void room::generateBullet(bool direction){
    bullet newAmmo(direction);
    bulletNode* tmp = new bulletNode();
    tmp->ammo = newAmmo;
    if(direction == LEFT){
        tmp->ammo.setColPos(protagonist.getColPos()-1);
        tmp->ammo.setRowPos(protagonist.getRowPos());
        if(!bulletCollision(tmp->ammo.getRowPos(), tmp->ammo.getColPos())){
            view[roomWidth * tmp->ammo.getRowPos()  + tmp->ammo.getColPos()] = tmp->ammo.getFigure();
            tmp->next = currentAmmo;
            currentAmmo = tmp;
        }
    }
    else{
        tmp->ammo.setColPos(protagonist.getColPos()+1);
        tmp->ammo.setRowPos(protagonist.getRowPos());
        if(!bulletCollision(tmp->ammo.getRowPos(), tmp->ammo.getColPos())){
            view[roomWidth * tmp->ammo.getRowPos()  + tmp->ammo.getColPos()] = tmp->ammo.getFigure();
            tmp->next = currentAmmo;
            currentAmmo = tmp;
        }
        
    }
    
}

void room::bulletMove(){
    bulletNode* iter = currentAmmo;
    while(iter != NULL){
        if(iter->ammo.getAlive()){
            if(iter->ammo.getDirection() == LEFT){
                //caso primo spawn ?? 
                //caso muro, il proiettile sparisce e viene setta a false
                if(roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()-1 == roomWidth * iter->ammo.getRowPos()){
                    view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = BLANK;
                    iter->ammo.setAlive();
                }
                //caso collisione con nemici o bonus
                else if(bulletCollision(iter->ammo.getRowPos(), iter->ammo.getColPos()-1)){
                    iter->ammo.setAlive();
                    view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = BLANK;
                }
                //Proiettile si muove di uno
                else{
                    view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = BLANK;
                    iter->ammo.setColPos(iter->ammo.getColPos()-1);
                    view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = iter->ammo.getFigure();
                }
            }
            else{
                if(view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()+1] == WALL){
                    iter->ammo.setAlive();
                    view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = BLANK;
                }
                else if(bulletCollision(iter->ammo.getRowPos(), iter->ammo.getColPos()+1)){
                    iter->ammo.setAlive();
                    view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = BLANK;
                }
                else{
                    view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = BLANK;
                    iter->ammo.setColPos(iter->ammo.getColPos()+1);
                    view[roomWidth * iter->ammo.getRowPos() + iter->ammo.getColPos()] = iter->ammo.getFigure();
                }
            }
        }
        iter = iter->next;
    }
}