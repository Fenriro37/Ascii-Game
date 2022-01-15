#include "Room.hpp"

hero protagonist;

//costruttore dummy, roomList
room::room(){
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
    currentAmmo = NULL;
    roomGenerator();
}

int room::toSingleArray(int x, int y){
    int total = roomWidth * x + y;
    return total;
}

//Funzione per bucare una serie di piattaforme
void room::drillRow(){

    for (int row = 2; row < roomHeight-1; row+=2) {
        int numberOfHoles = 0; 
        int position = 0;
        int maxHoles = roomNum/5 + 2;
        if (maxHoles > 5) maxHoles = 5;

        // Riga vuota
        if(freeRow[row-1].available){
            while(numberOfHoles < maxHoles){
                position = rand()%roomWidth;
                //primo buco, position != da: prima, seconda, ultima, penultima colonna
                if(position>1 && position<roomWidth-2){
                    newHoles(row, position, numberOfHoles, 1, roomWidth-2, false);
                }
            }
        }
        //Riga occupata da qualcosa
        else {
            // Caso 1 o 2 mostri
            if(freeRow[row-1].thereIsMonster){
                //caso un nemico
                 if(freeRow[row-1].numberOfMonsters == 1){
                    oneCastCase(row);  
                } 
                //caso due nemici
                // itero sulla riga per cercare i due mostri
                else {
                    twoMonstersCase(row, numberOfHoles);
                }  
            }
            // Caso bonus o torretta
            // divido la riga in 4 parti, buco quelle dove non c'è il qualcosa
            else {
                oneCastCase(row);
            } 
        }
    }    
}

void room::newHoles(int row, int position, int &holes, int start, int finish, bool alsoEqual){
    if(view[toSingleArray(row, position)] == PLATFORM){
        view[toSingleArray(row, position)] = BLANK;
        holes++;
        if(alsoEqual){
            if(position-1 >= start)
                view[toSingleArray(row, position-1)] = BLANK;
            if(position+1 <= finish)
                view[toSingleArray(row, position+1)] = BLANK;

        }
        else{
            if(position-1 > start)
                view[toSingleArray(row, position-1)] = BLANK;
            if(position+1 < finish)
                view[toSingleArray(row, position+1)] = BLANK;

        }
    }
}

void room::twoMonstersCase(int row, int holes){
    int yFirst = findYinRow(row-1, 0);
    int ySecond = findYinRow(row-1, 1);
    
    int lefter = (yFirst < ySecond) ? yFirst : ySecond;
    int righter = (yFirst < ySecond) ? ySecond : yFirst;
    int position = 0;
    //entrambi sono nella metà di sinistra
    if(righter <= roomWidth/2){
        while(holes < 2){
            position = roomWidth/2 + rand()%(roomWidth/2-3) +1;
            newHoles(row, position, holes, roomWidth/2, roomWidth-2, false);
        }
    }
    //entrambi sono nella metà di destra
    else if (lefter > roomWidth/2){
        while(holes < 2){
            position = rand()%(roomWidth/2-2)+2;
            newHoles(row, position, holes, 2, roomWidth/2, false);
        }
    }
    //sono in due metà diverse                    
    else{
        int section = (roomWidth-4)/4;
        if(lefter >= 1 && lefter <= 1+section){                                                                
            // Buchiamo nel SECONDO quarto
            //section + 2 per l'offset
            //rand()%(section-1) per non finire nel 3/4
            position = (section + 2) + rand()%(section);
            newHoles(row, position, holes, section + 2, 1 + 2*section, true);                        
        }
        // Buchiamo nel PRIMO quarto
        else if(lefter >= 2+section && lefter <=  1+2*section) {
            position =  2 + rand()%(section);
            newHoles(row, position, holes, 2, 1 + section, true);   
        }
        if(righter >= 2+2*section && righter  <= 3*section+1){                                                                
            // Buchiamo nel QUARTO quarto
            position = 2 + 3*section + rand()%(section);
            newHoles(row, position, holes, 2+ 3*section, 1+ 4*section, true);                            
        }
        else if(righter >= 2+3*section  && righter  <= 2+4*section) {
            // Buchiamo nel TERZO quarto
            position = 2 + 2*section + rand()%(section);
            newHoles(row, position, holes, 2+ 2*section, 1+ 3*section, true);    
        }                                                       
    }
}

//caso con un solo elemento nella riga
void room::oneCastCase(int row){
    int yOccupied = findYinRow(row-1, 0);    //posizione del qualcosa, findYinRow( , 0) per il primo
    int section = (roomWidth-4)/4;      //grandezza dei quarti di riga
    int holes = 0;
    //serve per trovare la posizione da bucare
    int position = 0;
    
    position = rand()%(roomWidth-3)+2;
    //if(position >= 2 && position <= 1+section)
    if(yOccupied > 1+section){
        position = rand()%section + 2;
        // cerchiamo di bucare nel  PRIMO quarto
        //something non è nel PRIMO quarto
        newHoles(row, position, holes, 2, 1+section, true);  
    }                        
    //else if(position >= 2+section && position <= 1+2*section){
    if(yOccupied < 2+section || yOccupied > 1+2*section){
        // cerchiamo di bucare nel SECONDO quarto
        //something non è nel SECONDO quarto
        position = rand()%section + 2 + section;
        newHoles(row, position, holes, 2+section, 1+ 2*section, true);  
    }                        
    //else if(position >= 2+2*section && position <= 3*section+1){
    if(yOccupied < 2+2*section || yOccupied > 3*section+1){
        // cerchiamo di bucare nel TERZO quarto
        //something non è nel TERZO quarto
        position = rand()%section + 2 + 2*section;
        newHoles(row, position, holes, 2+ 2*section, 1+ 3*section, true);  
    }                        
    //else if(position >= 2+3*section && position <= 1+4*section){
    if(yOccupied < 2+3*section){
        // cerchiamo di bucare nel QUARTO quarto
        //something non è nel QUARTO quarto
        position = rand()%section + 2 + 3*section;
        newHoles(row, position, holes, 2 + 3*section, 1+ 4*section, true);  
    }
}

//funzione ausiliaria per cercare nelle liste di enemy e di item 
//second = 0 cerco il primo, second = 1 cerco il secondo
int room::findYinRow(int row, int second){
    enemyNode* iterEnemy = currentMonsters;
    while(iterEnemy != NULL){
        if(iterEnemy->monster.getRowPos() == row){
            if(second>0)
                second=0;
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

//funzione per creare tutorial + stanze in maniera casuale
void room::roomGenerator(){
    //prima stanza con istruzioni (tutorial)
    if (roomNum == 0){
        for (int row = 0; row < roomHeight; row++) {
            for (int col = 0; col < roomWidth; col++) {
                //Istruzioni
                if (row == 2 && col == 3){
                    char field[13] = {'w','a','s','d',' ','t','o',' ','m','o','v','e', BLANK};
                    for(int i=0; i<13; i++, col++)
                        view[toSingleArray(row, col)] = field[i];
                }
                else if (row == 4 && col == 3){
                    char field[10] = {'p',' ','t','o',' ','p','a','u','s','e'};
                    for(int i=0; i<10; i++, col++)
                        view[toSingleArray(row, col)] = field[i];
                }
                else if (row == 6 && col == 3){
                    char field[15] = {'b','o','n','u','s','e','s',':',' ', HEART,' ', COIN, ' ', MAGAZINE, BLANK};
                    for(int i=0; i<15; i++, col++)
                        view[toSingleArray(row, col)] = field[i];
                }
                else if (row == 8 && col == 3){
                    char field[12] = {'e','n','e','m','i','e','s',':',' ', TURRET,' ', MONSTER};
                    for(int i=0; i<12; i++, col++)
                        view[toSingleArray(row, col)] = field[i];
                }
                //caso tetto
                else if (row == 0){ 
                    view[toSingleArray(row, col)] = ROOF;
                }
                //pavimento
                else if(row == roomHeight-1){
                    view[toSingleArray(row, col)] = FLOOR;
                }
                //caso Muro
                else if (col == 0 || col == roomWidth-1){ 
                    view[toSingleArray(row, col)] = WALL;
                }
                else if (row == roomHeight-3 && col >= 5 &&  col <= 18 + startColPos ){
                    view[toSingleArray(row, col)] = PLATFORM;
                }
                else {
                    view[toSingleArray(row, col)] = BLANK;
                }
            }
        }
        //modifiche manuali per angoli e "porte"
        view[0] = TOPLEFT;
        view[roomWidth-1] = TOPRIGHT;
        view[roomWidth * (roomHeight-2) + roomWidth-1] = BLANK;
        view[roomWidth * 8 + 15] = BLANK;
        view[roomWidth*(roomHeight-2)] = WALL;
        view[roomWidth*(roomHeight-1)] = BOTTOMLEFT;
        //inserimento protagonista
        view[roomWidth * startRowPos + startColPos] = protagonist.getFigure();
        initializeEnemies();
    }
    //stanze casuali
    else {
        for (int row = 0; row < roomHeight; row++) {
            for (int col = 0; col < roomWidth; col++) {
                //caso tetto
                if (row == 0){ 
                    view[toSingleArray(row, col)] = ROOF;
                }
                //pavimento
                else if(row == roomHeight-1){
                    view[toSingleArray(row, col)] = FLOOR;
                }
                //caso Muro
                else if (col == 0 || col == roomWidth - 1){ 
                view[toSingleArray(row, col)] = WALL;
                }
                //caso piattaforme
                else if (row%2==0){ 
                    view[toSingleArray(row, col)] = PLATFORM;
                }
                else {
                    view[toSingleArray(row, col)] = BLANK;
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

        initializeItems();
        initializeEnemies();

        spawnItems();
        spawnEnemies();

        drillRow();
    }
}

//Funzione per inizializzare gli item della stanza corrente
void room::initializeItems(){
    int numOfBonus = rand()%3;
    int count = 1;
    item newItem;
    currentBonus = new itemNode();
    currentBonus->Bonus = newItem;
    currentBonus->next = NULL;
    while((numOfBonus-1)!=0 && count < maxNumOfBonus){
        //inserimento in testa in una lista
        itemNode* tmp = new itemNode();
        item newItem;
        tmp->Bonus = newItem;
        tmp->next = currentBonus;
        currentBonus = tmp;
        count++; numOfBonus--;
    }
}

void room::initializeEnemies(){
    if(roomNum == 0){
        enemy monster(true);    //MONSTER
        monster.setRowPos(startRowPos);
        monster.setColPos(startColPos + 18);
        currentMonsters = new enemyNode();
        currentMonsters->monster = monster;
        currentMonsters->next = NULL;
        view[toSingleArray(monster.getRowPos(), monster.getColPos())] = monster.getFigure();
    }
    else{
        //Quanti nemici per stanza
        int numOfEnemies=2;
        if (roomNum<5) numOfEnemies = 2;
        else if (roomNum<15) numOfEnemies = 3;
        else if (roomNum<22) numOfEnemies = 4;
        else numOfEnemies = 5;
        
        enemy monster(true);    //MONSTER
        enemyNode* tmp = new enemyNode();
        tmp->monster = monster;
        tmp->next = NULL;
        currentMonsters = tmp;
        
        if(numOfEnemies == 5){
            enemy monster(true);    //MONSTER
            enemyNode* newMonster = new enemyNode();
            newMonster->monster = monster;
            newMonster->next = currentMonsters;
            currentMonsters = newMonster;
            numOfEnemies--;
        }

        numOfEnemies--;
        
        while(numOfEnemies > 0){
            //inserimento in testa in una lista
            enemyNode* tmp = new enemyNode();
            enemy newEnemy;
            tmp->monster = newEnemy;
            tmp->next = currentMonsters;
            currentMonsters = tmp;
            numOfEnemies--; 
        }
    }    
}

//controlla che la riga sia disponibile l'inserimento di un MONSTER:
//caso riga vuota o con un altro mostro al massimo
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

//controllo per lo spawn generico (MONSTER, TURRET, BONUS)
bool room::isAvailable(int x, int y, cast rookie){
    if(view[toSingleArray(x, y)] == BLANK){  
        //caso ultima riga (oltre la prima meta')
        if(x == roomHeight-2 && y >= roomWidth/2 && y < roomWidth-2){
            freeRow[x].available = false;
        }
        //non ultima riga, non mostro, riga senza mostri
        else if(rookie.getFigure() != MONSTER && x != roomHeight-2 && !freeRow[x].thereIsMonster && y > 1 && y < roomWidth-2){
            freeRow[x].available = false;
        }
        //caso mostro, non ultima riga
        else if(rookie.getFigure() == MONSTER && x != roomHeight-2 
        && y > 1 && y < roomWidth-2 
        //controllo che due MONSTER non spawnino attaccati: MM
        && view[toSingleArray(x, y+1)] != MONSTER && view[toSingleArray(x, y+2)] != MONSTER  
        && view[toSingleArray(x, y-1)] != MONSTER && view[toSingleArray(x, y-2)] != MONSTER){
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

void room::spawnItems(){
    int x, y;
    itemNode* iter = currentBonus;
    int empty[6] = {1,3,5,7,9,11};
    int rowGuess = rand()%6;
    while(iter != NULL){
        //remind il booleano taken parte sempre come falso
        if(!iter->Bonus.getTaken()){
            //fissiamo la x
            do{        
                x = empty[rowGuess];
                rowGuess = (rowGuess + 1)%6;
            }while(checkRow(x, iter->Bonus));
            //fissiamo la y
            do{
                y = rand()%(roomWidth-3)+2;
            }while(isAvailable(x,y, iter->Bonus));

            iter->Bonus.setRowPos(x);
            iter->Bonus.setColPos(y);
            view[toSingleArray(x, y)] = iter->Bonus.getFigure();
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
    int rowGuess = rand()%6;
    while(iter != NULL){
        if(iter->monster.getAlive()){
            do{
                x = empty[rowGuess];
                rowGuess = (rowGuess + 1)%6;
            }while(checkRow(x, iter->monster));
            
            do{
                y = rand()%(roomWidth-3)+2;
            }while(isAvailable(x,y, iter->monster));

            iter->monster.setRowPos(x);
            iter->monster.setColPos(y);
            view[toSingleArray(x, y)] = iter->monster.getFigure();
            iter = iter->next; 
        }
        else
            iter = iter->next;
    }
}

//trova l'item raccolto dal protagonista nella lista degli item, chiama setTaken()
itemNode* room::findBonus(int x, int y){
    itemNode* iter = currentBonus;
    while(iter != NULL){
        if(iter->Bonus.getRowPos() == x && iter->Bonus.getColPos() == y){
            iter->Bonus.setTaken();
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
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
    if(view[currentEnemy->monster.getPos() + offSet] == HERO){
        currentEnemy->monster.setAliveFalse();
        protagonist.decreaseLife();
        protagonist.setScore(-(roomNum * COLLISION_DMG_MULT)); 
        return true;
    }
    else if(view[currentEnemy->monster.getPos() + offSet] == BULLET){
        bulletNode* currentAmmo = findAmmo(currentEnemy->monster.getRowPos(), currentEnemy->monster.getColPos() + offSet);
        currentEnemy->monster.setAliveFalse();
        currentAmmo->ammo.setAliveFalse();
        protagonist.setScore(roomNum * BONUS_KILL_MULT);
        view[currentAmmo->ammo.getPos()] = BLANK;

        return true;
    }
    return false;
}

bulletNode* room::findAmmo(int x, int y){
    bulletNode* iter = currentAmmo;
    while(iter != NULL){
        if(iter->ammo.getRowPos() == x && iter->ammo.getColPos() == y && iter->ammo.getAlive()){
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
}

void room::generateBullet(bool direction, cast shooter){
    bullet newAmmo(direction);
    bulletNode* newBullet = new bulletNode();
    newBullet->ammo = newAmmo;
    newBullet->ammo.setColPos(shooter.getColPos());
    newBullet->ammo.setRowPos(shooter.getRowPos());
    int offSet = (direction == LEFT) ? -1 : 1;
    if(!bulletCollision(newBullet->ammo.getRowPos(), newBullet->ammo.getColPos() + offSet)){
        if(currentAmmo == NULL)
            currentAmmo = newBullet;
        else
            addToList(newBullet);
    } 
}

bool room::bulletCollision(int x, int y){            
    if(view[toSingleArray(x, y)] != BLANK){
        if(view[toSingleArray(x, y)] == MONSTER || view[toSingleArray(x, y)] == TURRET){
            enemyNode* foundMonster = findMoster(x, y);
            foundMonster->monster.setAliveFalse();
            protagonist.setScore(roomNum * BONUS_KILL_MULT);
            view[toSingleArray(x, y)] = BLANK;  
        }
        else if(view[toSingleArray(x, y)] == BULLET){
            bulletNode* foundBullet = findAmmo(x, y);
            foundBullet->ammo.setAliveFalse();
            view[toSingleArray(x, y)] = BLANK;
        }
        else if(view[toSingleArray(x, y)] == HERO){
            protagonist.decreaseLife();
            protagonist.setScore(-(roomNum * BULLET_DMG_MULT));
        }
        //i bonus se vengono colpiti sono distrutti
        else if(view[toSingleArray(x, y)] == HEART || view[toSingleArray(x, y)] == COIN || view[toSingleArray(x, y)] == MAGAZINE) {
            itemNode* foundBonus = findBonus(x, y);
            foundBonus->Bonus.setTaken();
            view[toSingleArray(x, y)] = BLANK;
        }
        return true;
    }
    return false;
}

//ottengo un puntatore all'inizio dell'array
char* room::getView(){
    return view;
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

bulletNode* room::getCurrentAmmo(){
    return currentAmmo;
}

enemyNode* room::getCurrentMonsters(){
    return currentMonsters;
}

void room::clearEnemies(){

    enemyNode* prev;
    //eliminiamo eventuali nodi in testa
    //questo primo blocco sembra funzionare: non crasha e elimina guardando col breakpoint
    while(currentMonsters != NULL && currentMonsters->monster.getAlive() != true){
        prev = currentMonsters;
        currentMonsters = currentMonsters->next;
        prev->next = NULL;
        delete prev;
    }
    //controlliamo il resto della lista
    //per uscire dal primo while o currentMonster è nullo e abbiamo finito o il suo mostro è vivo.
    //controlliamo allora il successivo per assegnarlo a enemy e a prev e controlliamo il resto della lista
    if(currentMonsters != NULL && currentMonsters->next != NULL){
        enemyNode* enemy = currentMonsters->next;
        prev = currentMonsters;
        while(enemy != NULL){
            if(enemy->monster.getAlive() == false){
                prev->next = enemy->next;
                enemy->next = NULL;
                delete enemy;
                enemy = prev->next;
            } 
            else{
                prev = enemy;
                enemy = enemy->next;
            }
        } 
    } 
}

void room::clearBonus(){

    itemNode* prev;
    while(currentBonus != NULL && currentBonus->Bonus.getTaken() != false){
        prev = currentBonus;
        currentBonus = currentBonus->next;
        prev->next = NULL;
        delete prev;
    }
    if(currentBonus != NULL && currentBonus->next != NULL){
        itemNode* bonus = currentBonus->next;
        prev = currentBonus;
        while(bonus != NULL){
            if(bonus->Bonus.getTaken() == true){
                prev->next = bonus->next;
                bonus->next = NULL;
                delete bonus;
                bonus = prev->next;
            } 
            else{
                prev = bonus;
                bonus = bonus->next;
            }
        } 
    } 
}

void room::clearBullet(){
    
    bulletNode* prev;
    while(currentAmmo != NULL && currentAmmo->ammo.getAlive() != true){
        prev = currentAmmo;
        currentAmmo = currentAmmo->next;
        prev->next = NULL;
        delete prev;
    }
    if(currentAmmo != NULL && currentAmmo->next != NULL){
        bulletNode* bullet = currentAmmo->next;
        prev = currentAmmo;
        while(bullet != NULL){
            if(bullet->ammo.getAlive() == false){
                prev->next = bullet->next;
                bullet->next = NULL;
                delete bullet;
                bullet = prev->next;
            } 
            else{
                prev = bullet;
                bullet = bullet->next;
            }
        } 
    } 
}

void room::clearList(){
    clearBonus();
    clearEnemies();
    clearBullet();
}
