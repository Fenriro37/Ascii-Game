#include "Room.hpp"
#include <string>

room::room(){
    roomNum = 1;
    roomGenerator();
   
    currentBonus = NULL;
    currentMonsters = NULL;
    next = NULL;
    prev = NULL;
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
            platforms[random] = ' ';
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
            //caso tetto e pavimento
            if (row == 0 || row == roomHeight - 1){ 
                view[row][col] = '#';
            }
        /*generazione delle righe con le piattaforme che si troveranno solo sulle righe pari diverse da 0
         e dall'ultima*/
            else if ((row%2==0) && row!=0 && row != roomHeight - 1){ 
                view[row][col] = platforms[col];
            }
            else {
                view[row][col] = ' ';
            }    
        }
    }
    //inserimento protagonista
    view[protagonist.getRowPos()][protagonist.getColPos()] = protagonist.getFigure();

    /*  TODO: aggiornare initialize item con bonus casuali
          ora tutti i bonus sono copie 
    */

    initializeItems(getRoomNum());
    initializeEnemies(getRoomNum());

    spawnEnemies();
    spawnItems();
    
    toCharInfo();
}

/*
* Doing sizeof myArray will get you the total number of bytes allocated for that array.
* You can then find out the number of elements in the array  by dividing
* by the size of one element in the array: sizeof myArray[0]
*/
int room::sizeofArray(char charArray[]){
    return sizeof(charArray)/sizeof(charArray[0]);
}

void room::paste (char arrayToPaste[], int &count){
        for (int i=0; arrayToPaste[i]!='\0'; i++){
            CIview[count].Char.AsciiChar = arrayToPaste[i];
            CIview[count++].Attributes = DEF_COLORFOREGROUND;
        }
    }

//funzione per convertire da char a charinfo per poter essere rappresentato tramite la libreria windows.h
void room::toCharInfo() {    

    int count = 0;
    for (int row = 0; row < consoleHeight; row++) {
        for (int col = 0; col < consoleWidth; col++) {
            if (col < roomWidth  && row < roomHeight) {
                CIview[count].Char.AsciiChar = view[row][col];
                CIview[count].Attributes = DEF_COLORFOREGROUND;
            }
            /*
            * lvl, hp, score, ammo */
            else if (row == 4 && col == roomWidth+3){
                char LVL[] = {'L','V','L',':','\0'};
                paste(LVL, count);
            }
            else { 
                CIview[count].Char.AsciiChar = ' ';
                CIview[count].Attributes = DEF_COLORFOREGROUND;
            }
            count++;
        }
    }
}

void room::setRoomNum(int newRoomNum){
    roomNum = newRoomNum;
}

int room::getRoomNum(){
    return roomNum;
}

void room::setView(char newView[roomHeight][roomWidth]){    //##cos'è?
    //view = newView;
    for(int i=0; i < roomHeight; i++){
        for(int j=0; j < roomHeight; j++)
            view[i][j] = newView[i][j];
    }
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
    if(x%2 != 0 && view[x][y] == ' ' && view[x+1][y] != ' ')   
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
                view[x][y] = iter->Bonus.getFigure();
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
        if(!iter->monster.getAlive()){
            x = rand()%roomHeight;
            y = rand()%roomWidth;
            if(isEmpty(x,y)){
                iter->monster.setRowPos(x);
                iter->monster.setColPos(y);
                view[x][y] = iter->monster.getFigure();
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

void room::enemyMove(){ //movimento orizzontale
    enemyNode* iter = currentMonsters;
    while(iter != NULL){
        if(!iter->monster.getAlive()){
            if(!iter->monster.getDirection()){
                //0 per movimento a sx
                if(view[iter->monster.getRowPos()+1][iter->monster.getColPos()-1] != ' ' 
                    && view[iter->monster.getRowPos()][iter->monster.getColPos()-1] != '#'){
                        view[iter->monster.getRowPos()][iter->monster.getColPos()] = ' ';
                        iter->monster.setColPos(iter->monster.getColPos()-1);
                        view[iter->monster.getRowPos()][iter->monster.getColPos()] = iter->monster.getFigure();
                }
                else {
                    iter->monster.setDirection();
                }
            }
            else {
                //1 per movimento a dx
                if(view[iter->monster.getRowPos()+1][iter->monster.getColPos()+1] != ' ' 
                    && view[iter->monster.getRowPos()][iter->monster.getColPos()+1] != '#'){
                        view[iter->monster.getRowPos()][iter->monster.getColPos()] = ' ';
                        iter->monster.setColPos(iter->monster.getColPos()+1);
                        view[iter->monster.getRowPos()][iter->monster.getColPos()] = iter->monster.getFigure();
                }
                else {
                    iter->monster.setDirection();
                }
            }
        }
        iter = iter->next;
    }
}