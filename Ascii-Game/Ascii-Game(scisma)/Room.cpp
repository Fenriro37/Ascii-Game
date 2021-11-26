#include "Room.hpp"
#include <string>
#include "iostream"
#include <cmath>

room::room(){
    //costruttore dummy
    nextLevelPos();
}

room::room(int lvl){
    roomNum = lvl;
    for (int i=0;i<roomHeight;i++){
        if(i%2 == 0)
            freeRow[i].available = false;        
        else
            freeRow[i].available = true;
        freeRow[i].thereIsMonster = false;
        freeRow[i].numberOfMonsters = 0;
    }
    currentMonsters = NULL;
    currentBonus = NULL;
    roomGenerator();
}

//Funzione per bucare randomicamente una serie di piattaforme
void room::drillRow(const int currentLevel){

    for (int row = 2; row < roomHeight-1; row+=2) {

        int numberOfHoles = 0; 
        int position = 0;
        int maxHoles = currentLevel/5 + 2;
        if (maxHoles > 5) maxHoles = 5;

        /* Riga vuota
        */
        if(freeRow[row-1].available){
            while(numberOfHoles < maxHoles){
                position = rand()%roomWidth;
                //primo buco, position != da: prima, seconda, ultima, penultima colonna
                if(view[roomWidth * row + position] == PLATFORM && position!=1 && position!=roomWidth-2){
                    view[roomWidth * row + position] = BLANK;
                    numberOfHoles++;
                    if(position-1>1)
                        view[roomWidth * row + position-1] = BLANK;
                    if(position+1 < roomWidth-2)
                        view[roomWidth * row + position+1] = BLANK;
                }
            }
        }
        /* Riga occupata da qualcosa
        */
        else {
            /* Caso 1 o 2 mostri
            itero sulla riga per cercare i due mostri
            struct sese
            */
            if(freeRow[row-1].thereIsMonster){
                //caso un nemico
                if(freeRow[row-1].numberOfMonsters == 1){
                    thirdCase(row, numberOfHoles, position, maxHoles);  
                }
                //caso due nemici
                else {
                    int yFirst = findYinRow(row-1, 0);
                    int ySecond = findYinRow(row-1, 1);
                    int distanceBtw = abs(yFirst - ySecond);
                    
                    int lefter = (yFirst < ySecond) ? yFirst:ySecond;
                    int righter = (yFirst < ySecond) ? ySecond:yFirst;
                    //entrambi sono nella metà di sinistra
                    if(righter < roomWidth/2){
                        while(numberOfHoles < 2){
                            position = roomWidth - rand()%(roomWidth/2 +2);
                            if(view[roomWidth * row + position] == PLATFORM){
                                view[roomWidth * row + position] = BLANK;
                                numberOfHoles++;
                                if(position+1 < roomWidth-2)
                                    view[roomWidth * row + position+1] = BLANK;
                                if(position-1 > roomWidth/2)
                                    view[roomWidth * row + position-1] = BLANK;
                            }
                        }
                    }
                    //entrambi sono nella metà di destra
                    else if (lefter > roomWidth/2){
                        while(numberOfHoles < 2){
                            position = rand()%(roomWidth/2 +2);
                            if(view[roomWidth * row + position] == PLATFORM){
                                view[roomWidth * row + position] = BLANK;
                                numberOfHoles++;
                                if(position+1 < roomWidth/2)
                                    view[roomWidth * row + position+1] = BLANK;
                                if(position-1 > 2)
                                    view[roomWidth * row + position-1] = BLANK;
                            }
                        }
                    }
                    //sono in due metà diverse                    
                    //caso: sono "vicini"  circa 13
                    else{
                        int section = (roomWidth-4)/4;
                        //while(numberOfHoles < 2){   //forse inutile
                            if(yFirst >= 2 && yFirst <= 1+section){                                                                
                                // Buchiamo nel SECONDO quarto
                                //section + 2 per l'offset
                                //rand()%(section-1) per non finire nel 3/4
                                position = (section + 2) + rand()%(section);
                                if(view[roomWidth * row + position] == PLATFORM){
                                    view[roomWidth * row + position] = BLANK;
                                    numberOfHoles++;
                                    if(position-1 >= section + 2)
                                        view[roomWidth * row + position-1] = BLANK;
                                    if(position+1 <= 1 + 2*section)
                                        view[roomWidth * row + position+1] = BLANK;
                                }                            
                            }
                            // Buchiamo nel PRIMO quarto
                            else if(yFirst >= 2+section && yFirst <=  1+2*section) {
                                position =  2 + rand()%(section);
                                if(view[roomWidth * row + position] == PLATFORM){
                                    view[roomWidth * row + position] = BLANK;
                                    numberOfHoles++;
                                    if(position-1 >=  2)
                                        view[roomWidth * row + position-1] = BLANK;
                                    if(position+1 <= 1+section)
                                        view[roomWidth * row + position+1] = BLANK;
                                }  
                            }
                            if(ySecond >= 2+2*section && ySecond  <= 3*section+1){                                                                
                                // Buchiamo nel QUARTO quarto
                                position = 2*section + 2 + rand()%(section);
                                if(view[roomWidth * row + position] == PLATFORM){
                                    view[roomWidth * row + position] = BLANK;
                                    numberOfHoles++;
                                    if(position-1 >= 2+2*section)
                                        view[roomWidth * row + position-1] = BLANK;
                                    if(position+1 <= 3*section+1)
                                        view[roomWidth * row + position+1] = BLANK;
                                }                            
                            }
                            else if(ySecond >= 2+3*section  && ySecond  <= 1+4*section) {
                                // Buchiamo nel TERZO quarto
                                position =  2 + 3*section + rand()%(section);
                                if(view[roomWidth * row + position] == PLATFORM){
                                    view[roomWidth * row + position] = BLANK;
                                    numberOfHoles++;
                                    if(position-1>= 2+3*section)
                                        view[roomWidth * row + position-1] = BLANK;
                                    if(position+1 <= 1+4*section)
                                        view[roomWidth * row + position+1] = BLANK;
                                }  
                            }                               
                        }
                    }
                }
            }
            /* Caso bonus o torretta
            divido la riga in 4 parti, buco quelle dove non c'è il qualcosa
            */
            else {
                thirdCase(row, numberOfHoles, position, maxHoles);
            }
        }
    }    
}

//caso con un solo elemento nella riga
void room::thirdCase(int row, int holes, int position, int maxHoles){
    int yOccupied = findYinRow(row-1, 0);    //posizione del qualcosa, findYinRow( , 0) per il primo
    int section = (roomWidth-4)/4;      //grandezza dei quarti di riga

    while(holes < maxHoles){
        position = rand()%roomWidth;
        if(position >= 2 && position <= 1+section){
            if(yOccupied == 1 || yOccupied > 1+section){
                // cerchiamo di bucare nel  PRIMO quarto
                //something non è nel PRIMO quarto
                if(view[roomWidth * row + position] == PLATFORM){
                    view[roomWidth * row + position] = BLANK;
                    holes++;
                    if(position-1 >= 2)
                        view[roomWidth * row + position-1] = BLANK;
                    if(position+1 <= 1+section)
                        view[roomWidth * row + position+1] = BLANK;
                }
            }                        
        }
        else if(position >= 2+section && position <= 1+2*section){
            if(yOccupied < 2+section || yOccupied > 1+2*section){
                // cerchiamo di bucare nel SECONDO quarto
                //something non è nel SECONDO quarto
                if(view[roomWidth * row + position] == PLATFORM){
                    view[roomWidth * row + position] = BLANK;
                    holes++;
                    if(position-1 >= 2+section)
                        view[roomWidth * row + position-1] = BLANK;
                    if(position+1 <= 1+2*section)
                        view[roomWidth * row + position+1] = BLANK;
                }
            }                        
        }
        else if(position >= 2+2*section && position <= 3*section+1){
            if(yOccupied < 2+2*section || yOccupied > 3*section+1){
                // cerchiamo di bucare nel TERZO quarto
                //something non è nel TERZO quarto
                if(view[roomWidth * row + position] == PLATFORM){
                    view[roomWidth * row + position] = BLANK;
                    holes++;
                    if(position-1 >= 2+2*section)
                        view[roomWidth * row + position-1] = BLANK;
                    if(position+1 <= 3*section+1)
                        view[roomWidth * row + position+1] = BLANK;
                }
            }                        
        }
        else if(position >= 2+3*section && position <= 1+4*section){
            if(yOccupied < 2+3*section || yOccupied == 2+4*section){
                // cerchiamo di bucare nel QUARTO quarto
                //something non è nel QUARTO quarto
                if(view[roomWidth * row + position] == PLATFORM){
                    view[roomWidth * row + position] = BLANK;
                    holes++;
                    if(position-1 >= 2+3*section)
                        view[roomWidth * row + position-1] = BLANK;
                    if(position+1 <= 1+4*section)
                        view[roomWidth * row + position+1] = BLANK;
                }
            }
        }
    }
}

//funzione ausiliaria per cercare nelle liste di enemy e di item 
//second = 0 cerco il primo, second = 1 cerco il secondo
int room::findYinRow(int row, int second){
    enemyNode* iterEnemy = currentMonsters;
    while(iterEnemy != NULL){
        if(iterEnemy->monster.getRowPos() == row){
            if(second)
                second--;
            else
                return iterEnemy->monster.getColPos();
        }
        iterEnemy = iterEnemy->next;
    }

    itemNode* iterItem = currentBonus;
    while(iterItem != NULL){
        if(iterItem->Bonus.getRowPos() == row){            
            return iterItem->Bonus.getColPos();
        }
        iterItem = iterItem->next;
    }
    return 0;
}

//funzione per creare una stanza in maniera casuale
void room::roomGenerator(){
    for (int row = 0; row < roomHeight; row++) {
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
            else if (row%2==0){ 
                view[roomWidth * row + col] = PLATFORM;
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

    drillRow(roomNum);
    
}

//Funzione per inizializzare gli item della stanza corrente
//Quanti nemici contemporaneamente?1K
void room::initializeItems(int currentLevel){
    // O mettiamo un certo numero di bonus fissi per incentivare lo spostamento o ci affidiamo al caso
    int numOfBonus =  2;//rand()%2+1; //currentLevel / 3 + 1 || 
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
    int numOfEnemies = 3;//rand()%3+1;
    int count = 1;
    enemy monster;
    currentMonsters = new enemyNode();
    currentMonsters->monster = monster;
    currentMonsters->next = NULL;
    while((numOfEnemies-1)>0 && count<=maxNumOfEnemies){
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
    if(view[roomWidth * x + y] == BLANK){  
        //caso ultima riga (oltre la prima meta')
        if(x == roomHeight-2 && y >= roomWidth/2 && y != roomWidth-1){
            freeRow[x].available = false;
        }
        //non ultima riga, non mostro, riga senza mostri
        else if(rookie.getFigure() != MONSTER && x != roomHeight-2 && !freeRow[x].thereIsMonster){
            freeRow[x].available = false;
        }
        //caso mostro, non ultima riga
        else if(rookie.getFigure() == MONSTER && x != roomHeight-2){
            freeRow[x].available = false;        
            freeRow[x].thereIsMonster = true;
        }
        else {
            return true;
        }

        return false;
    }
    return true;
}

bool room::checkRow(int row, cast character){
    if(freeRow[row].available){
        if(character.getFigure() == MONSTER)
            freeRow[row].numberOfMonsters++;
        return false;
    }
    else if (character.getFigure() == MONSTER && freeRow[row].thereIsMonster && freeRow[row].numberOfMonsters < 2){
        freeRow[row].numberOfMonsters++;
        return false;
    }
    else return true;
}

void room::spawnItems(){
    int x, y;
    itemNode* iter = currentBonus;
    int empty[6] = {1,3,5,7,9,11};
    while(iter != NULL){
        //remind il booleano taken parte sempre come falso
        if(!iter->Bonus.getTaken()){
            //fissiamo la x
            do{
                x = empty[rand()%6];
            }while(checkRow(x, iter->Bonus));
            //fissiamo la y
            do{
                y = rand()%roomWidth;
            }while(isAvailable(x,y, iter->Bonus));

            iter->Bonus.setRowPos(x);
            iter->Bonus.setColPos(y);
            view[roomWidth * x + y] = iter->Bonus.getFigure();
            iter = iter->next;
            }
        else
            iter = iter->next;
    }
}

void room::spawnEnemies(){
    int x, y;
    enemyNode* iter = currentMonsters;
    int empty[6] = {1,3,5,7,9,11};
    while(iter != NULL){
        //remind i mostri partono sempre con il bool alive = true
        //torretta si prende una riga per sè
        if(!iter->monster.getAlive()){
            do{
                x = empty[rand()%6];
            }while(checkRow(x, iter->monster));
            
            do{
                y = rand()%roomWidth;
            }while(isAvailable(x,y, iter->monster));

            iter->monster.setRowPos(x);
            iter->monster.setColPos(y);
            view[roomWidth * x + y] = iter->monster.getFigure();
            iter = iter->next; 
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
        protagonist.setScore(-(roomNum * roomNum * 0.01 + 20));
        return true;
    }
    else if(view[roomWidth * currentEnemy->monster.getRowPos() + (currentEnemy->monster.getColPos() + offSet)] == BULLET){
        bulletNode* currentAmmo = findAmmo(currentEnemy->monster.getRowPos(), currentEnemy->monster.getColPos() + offSet);
        currentEnemy->monster.setAlive();
        currentAmmo->ammo.setAlive();
        protagonist.setScore(5 * roomNum + 50);
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
    int offSet = (direction == LEFT) ? -1 : 1;
    if(!bulletCollision(newBullet->ammo.getRowPos(), newBullet->ammo.getColPos() + offSet)){
        if(currentAmmo == NULL){
            currentAmmo = newBullet;
        }
        else{
            addToList(newBullet);
        }
    } 
}

bool room::bulletCollision(int x, int y){
    if(view[roomWidth * x + y] != BLANK){
        if(view[roomWidth * x + y] == MONSTER || view[roomWidth * x + y] == TURRET){
            enemyNode* foundMonster = findMoster(x, y);
            foundMonster->monster.setAlive();
            protagonist.setScore( 5* roomNum + 50);
            view[roomWidth * x + y] = BLANK;  
        }
        else if(view[roomWidth * x + y] == BULLET){
            view[roomWidth * x + y] = BLANK;
        }
        else if(view[roomWidth * x + y] == HERO){
            protagonist.decreaseLife();
            protagonist.setScore(-(roomNum * roomNum * 0.01 + 20));
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