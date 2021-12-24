#include "Game.hpp"
#include "conio.h"
#include "string.h"
game::game() {

    /* Console stuff
        ############################
    */ 

    /* Window size coordinates, be sure to start index at zero! */
    consoleSize = { 0, 0, consoleWidth - 1, consoleHeight - 1 };

    /* A COORD struct for specificying the console's screen buffer dimensions */
    COORD bufferSize = { consoleWidth, consoleHeight };

    /* Setting up different variables for passing to WriteConsoleOutput */
    characterBufferSize = { consoleWidth, consoleHeight };
    characterPosition = { 0, 0 };
    //SMALL_RECT consoleWriteArea = {0, 0, consoleWidth - 1, consoleHeight - 1};

    /* A CHAR_INFO structure containing data about a single character */
    CHAR_INFO consoleBuffer[consoleWidth * consoleHeight];

    /* initialize handles */
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    /* Set the console's title */
    //SetConsoleTitle("Our shiny new title!");
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(hStdout, &cursor);
	//infoCursore.bVisible = visible;
    cursor.bVisible = false;
	SetConsoleCursorInfo(hStdout, &cursor);

    /* Set the window size */
    SetConsoleWindowInfo(hStdout, TRUE, &consoleSize);

    /* Set the screen's buffer size */
    SetConsoleScreenBufferSize(hStdout, bufferSize);

    CONSOLE_FONT_INFOEX font = {sizeof(CONSOLE_FONT_INFOEX)};//CONSOLE_FONT_INFOEX is defined in some windows header
    GetCurrentConsoleFontEx(hStdout, false, &font);//PCONSOLE_FONT_INFOEX is the same as CONSOLE_FONT_INFOEX*
    font.dwFontSize.X = 24;
    font.dwFontSize.Y = 35;
    SetCurrentConsoleFontEx(hStdout, false, &font);

    currentroom = new roomList(1);
    score = 50;
}

/*Otteniamo la posizione della cella da cambiare e la sostituiamo con figure
* Funziona (forse) perchè gli array sono memoria contigua quindi spostandoci di indirizzo senza uscire da 
*/
void game::changeCellOfView(int position, char figure){
    if(position<= roomWidth*roomHeight && position >= 0){
        char* PtoArray = currentroom->myRoom.getView();
        PtoArray += position;
        *PtoArray = figure;
    }
}

void game::clearList(){
    clearBonus();
    clearEnemy();
    clearAmmo();
}

void game::clearBonus(){
    itemNode* bonus  = currentroom->myRoom.getCurrentBonus();
    itemNode* prev;
    prev = bonus;
    while(bonus != NULL){
        //caso testa. La condizione ci permette di controllare se siamo ancora la testa
        if (bonus->Bonus.getTaken() && prev == bonus){
            if(bonus->next != NULL){                                
                bonus = bonus->next;
                //delete prev;
                prev = bonus;

            }
            //testa unico nodo
            else {
                //delete bonus;
                currentroom->myRoom.setCurrentBonus(NULL);
                bonus = NULL;
                prev = NULL;
            }
        }
        //se non devo eliminare il primo sposto di uno bonus da prev
        else if (!bonus->Bonus.getTaken() && prev == bonus){  
            currentroom->myRoom.setCurrentBonus(bonus);
            bonus = bonus->next;
        }
        //se devo eliminare un nodo che non è in testa
        else if (bonus->Bonus.getTaken() && prev != bonus){
            prev->next = bonus->next;
            //delete bonus;
            bonus = prev->next;
        }
        //se non devo eliminare un nodo non in testa
        else{
            prev = bonus;
            bonus = bonus->next;
        }   
    }
}

void game::clearEnemy(){
    enemyNode* enemy  = currentroom->myRoom.getCurrentMonsters();
    enemyNode* prev;
    prev = enemy;
    while(enemy != NULL){
        //caso testa. La condizione ci permette di controllare se siamo ancora la testa
        if (enemy->monster.getAlive() && prev == enemy){
            if(enemy->next != NULL){                                
                enemy = enemy->next;
                //delete prev;
                prev = enemy;

            }
            //testa unico nodo
            else {
                //delete bonus;
                currentroom->myRoom.setCurrentMonster(NULL);
                enemy = NULL;
                prev = NULL;
            }
        }
        //se non devo eliminare il primo sposto di uno bonus da prev
        else if (!enemy->monster.getAlive() && prev == enemy){  
            currentroom->myRoom.setCurrentMonster(enemy);
            enemy = enemy->next;
        }
        //se devo eliminare un nodo che non è in testa
        else if (enemy->monster.getAlive() && prev != enemy){
            prev->next = enemy->next;
            //delete bonus;
            enemy = prev->next;
        }
        //se non devo eliminare un nodo non in testa
        else{
            prev = enemy;
            enemy = enemy->next;
        }   
    }
}

void game::clearAmmo(){
    bulletNode* bullet  = currentroom->myRoom.getCurrentAmmo();
    bulletNode* prev;
    prev = bullet;
    while(bullet != NULL){
        //caso testa. La condizione ci permette di controllare se siamo ancora la testa
        if (bullet->ammo.getAlive() && prev == bullet){
            if(bullet->next != NULL){                                
                bullet = bullet->next;
                //delete prev;
                prev = bullet;

            }
            //testa unico nodo
            else {
                //delete bonus;
                currentroom->myRoom.setCurrentAmmo(NULL);
                bullet = NULL;
                prev = NULL;
            }
        }
        //se non devo eliminare il primo sposto di uno bonus da prev
        else if (!bullet->ammo.getAlive() && prev == bullet){  
            currentroom->myRoom.setCurrentAmmo(bullet);
            bullet = bullet->next;
        }
        //se devo eliminare un nodo che non è in testa
        else if (bullet->ammo.getAlive() && prev != bullet){
            prev->next = bullet->next;
            //delete bonus;
            bullet = prev->next;
        }
        //se non devo eliminare un nodo non in testa
        else{
            prev = bullet;
            bullet = bullet->next;
        }   
    }
}
/*Controlla che la posizione sia uguale a figure
*/
bool game::checkNear(int row, int col, char figure){
    char* PtoArray = currentroom->myRoom.getView();
    PtoArray += roomWidth*row+col;
    return (*PtoArray != figure);
}

void game::nextRoom(){

    //qui aggiorniamo le liste rimuovendo i nodi settati a true (morti o raccolti)
    clearList();

    changeCellOfView(protagonist.getPos(), BLANK);
    if (currentroom->next == NULL){
        protagonist.setScore(50);
        currentroom->next = new roomList(currentroom->myRoom.getRoomNum() + 1);
        currentroom->next->prev = currentroom;

        currentroom = currentroom->next;
    }
    else {
        currentroom = currentroom->next;
        currentroom->myRoom.nextLevelPos();
        changeCellOfView(protagonist.getPos(), protagonist.getFigure());
    }
}

//Quando il personaggio vuole tornare indietro
void game::prevRoom(){
    clearList();

    if (currentroom->prev != NULL){
        changeCellOfView(protagonist.getPos(), BLANK);
        currentroom = currentroom->prev;
        currentroom->myRoom.prevLevelPos();
        changeCellOfView(protagonist.getPos(), protagonist.getFigure());
    }
}

void game::stampView() {    
    WriteConsoleOutputA(hStdout, CIview, characterBufferSize, characterPosition, &consoleSize);
}

/*
//Funzione per gestire spostamento, cambio stanza, impatto e nemici
*/
void game::logic(){

    if(kbhit()){
        move(getch());
        if(protagonist.getColPos() == roomWidth - 1 && protagonist.getRowPos() == roomHeight-2){
            nextRoom();
        }
        if (protagonist.getColPos() == 0 && protagonist.getRowPos() == roomHeight-2)
            prevRoom(); 
    }
    Sleep(50);

    currentroom->myRoom.bulletMove();
    currentroom->myRoom.enemyMove();

    gameOver();
    stampView();
    while(1){int i;}
}

//Il caso A è il più commentato perchè stato fatto per primo. Per dubbi riferirsi a quello
void game::move(char input){
    switch(input){
        /*Movimento Sopra e Sotto
         */
        case 'w': 
        case 'W': if(checkNear(protagonist.getRowPos()-1, protagonist.getColPos(), BLANK) && 
                     checkNear(protagonist.getRowPos()-1, protagonist.getColPos(), ROOF)){
                        changeCellOfView(protagonist.getPos(), BLANK);
                        protagonist.setRowPos(protagonist.getRowPos()-2);   //sale di piattaforma -> -2
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos());
                        changeCellOfView(protagonist.getPos(), protagonist.getFigure());
                    } 
            break;
        case 's':    //+3 per controllare che ci sia la piattaforma sotto i piedi 
        case 'S': if(checkNear(protagonist.getRowPos()+3, protagonist.getColPos(), BLANK) &&
                     checkNear(protagonist.getRowPos()+1, protagonist.getColPos(), FLOOR) ){
                        changeCellOfView(protagonist.getPos(), BLANK);  
                        protagonist.setRowPos(protagonist.getRowPos()+2);
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos());
                        changeCellOfView(protagonist.getPos(), protagonist.getFigure());
                    }
            break;
        /*Movimento Sinistra e Destra*/    
        case 'a': 
        case 'A': if(checkNear(protagonist.getRowPos()+1, protagonist.getColPos()-1, BLANK) &&
                     checkNear(protagonist.getRowPos(), protagonist.getColPos()-1, WALL)){
                        changeCellOfView(protagonist.getPos(), BLANK);
                        //change into new value
                        protagonist.setColPos(protagonist.getColPos()-1);
                        //controlliamo la posizione su cui ci spostiamo
                        //se impattiamo contro porta
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos());
                        changeCellOfView(protagonist.getPos(), protagonist.getFigure()); 
                        //altrimente se ci siamo scontrati con nulla o con item o nemici 
                    }
            break;
        case 'd': 
        case 'D': if(checkNear(protagonist.getRowPos()+1, protagonist.getColPos()+1, BLANK) &&
                     checkNear(protagonist.getRowPos(), protagonist.getColPos()+1, WALL)){
                        changeCellOfView(protagonist.getPos(), BLANK);
                        protagonist.setColPos(protagonist.getColPos()+1);
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos());
                        changeCellOfView(protagonist.getPos(), protagonist.getFigure()); 
                    } 
            break;
        
        case 'j': //Proiettile sinistra
        case 'J': if(protagonist.getBullet() > 0 && checkNear(protagonist.getRowPos(), protagonist.getColPos()-1, WALL) && 
                     roomWidth*protagonist.getRowPos()+protagonist.getColPos()-1 != roomWidth*(roomHeight-2)){
                    protagonist.decreaseBullet();
                    currentroom->myRoom.generateBullet(LEFT, protagonist);
                  }
            break;
                //Proiettile destra
        case 'k':if(protagonist.getBullet() > 0  && checkNear(protagonist.getRowPos(), protagonist.getColPos()+1, WALL) && 
                     roomWidth*protagonist.getRowPos()+protagonist.getColPos()+1 != roomWidth*(roomHeight-2)+roomWidth-1){
                    protagonist.decreaseBullet();
                    currentroom->myRoom.generateBullet(RIGHT, protagonist);
                  }                  
            break;

        default:    
            break;
    }
}

//trova l'item raccolto dal protagonista nella lista degli item, chiama setTaken()
void game::findItem(int row, int col){
    itemNode* iter = currentroom->myRoom.getCurrentBonus();
    while(iter!= NULL){
        if(iter->Bonus.getRowPos() == row && iter->Bonus.getColPos() == col){
            iter->Bonus.setTaken();
            break;
        }
        iter = iter->next;
    }
}

//Funzione per controllare l'interazione con mostri e items
//Gestisce le vite e lo score
//Aggiornare con le collisioni dei proiettili o aggiungere un'altra funzione
//Funzioni findMonster() e findBonus() da utilizzare al posto del while
void game::playerCollision(int row, int col){
    if(checkNear(row, col, BLANK)){
        //caso collisione enemy
        if(!checkNear(row, col, MONSTER) || !checkNear(row, col, TURRET)){
            protagonist.decreaseLife();   
            protagonist.setScore(-(currentroom->myRoom.getRoomNum() * currentroom->myRoom.getRoomNum() * 0.01 + 20));
            
            //dobbiamo muoverci nella lista per vedere con quale nemico ci siamo scontrati
            enemyNode* iter;
            iter = currentroom->myRoom.findMoster(row, col);
            iter->monster.setAlive();
        }
        //caso bullet 
        //aggiungere proiettile nemico
        else if(!checkNear(row, col, BULLET)){
            bulletNode* iter;
            iter = currentroom->myRoom.findAmmo(row, col);
            iter->ammo.setAlive();
            protagonist.decreaseLife();
            protagonist.setScore(-(currentroom->myRoom.getRoomNum() * currentroom->myRoom.getRoomNum() * 0.01 + 20));
        }
        //caso item
        //Se non era un mostro dobbiamo controllare quale bonus si trovava in quella posizione
        else{
            if(!checkNear(row, col, HEART)){
                findItem(row, col);
                protagonist.setLife(protagonist.getLife() + 1);
            }
            else if(!checkNear(row, col, MAGAZINE)){
                findItem(row, col);
                protagonist.setBullet(protagonist.getBullet() + 3);           //########### NB
            }
            else if(!checkNear(row, col, COIN)){
                findItem(row, col);
                protagonist.setScore(10* currentroom->myRoom.getRoomNum() + 20); 
               
            }
        }
    }
}

void game::paste (char arrayToPaste[], int size, int &count, int &col){
    for (int i=0; i<size-1; i++){
        CIview[count].Char.AsciiChar = arrayToPaste[i];
        CIview[count++].Attributes = DEF_COLORFOREGROUND;
        col++;
    }
    CIview[count].Char.AsciiChar = arrayToPaste[size-1];
    CIview[count].Attributes = DEF_COLORFOREGROUND;
}

//funzione per convertire da char a charinfo per poter essere rappresentato tramite la libreria windows.h
void game::toCharInfo() {   
    int count = 0;
    for (int row = 0; row < consoleHeight; row++) {
        for (int col = 0; col < consoleWidth; col++, count++) {
            if (col < roomWidth  && row < roomHeight) {
                char* tmp = currentroom->myRoom.getView();
                tmp += roomWidth * row + col;
                CIview[count].Char.AsciiChar = *tmp;
                CIview[count].Attributes = DEF_COLORFOREGROUND;
            }
            /*
            * lvl, hp, score, ammo */
            else if (row == 2 && col == roomWidth+10){
                char field[8] = {'L','V','L',':',BLANK};
                char value[3];
                /* _itoa() funzione che converte il primo argomento (intero) in stringa
                * e lo salva nel secondo argomento, il terzo argomento è la base di conversione
                */
                _itoa(currentroom->myRoom.getRoomNum(), value, 10);
                strcat(field, value);
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == 4 && col == roomWidth+10){
                char field[13] = {'S','C','O','R','E',':',BLANK};
                char value[6]; //fino a 999.999
                _itoa(protagonist.getScore() , value, 10);
                strcat(field, value);
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == 7 && col == roomWidth+10){
                char field[6] = {'H','P',':',BLANK};
                char value[2]; //fino a 99
                _itoa(protagonist.getLife(), value, 10);
                strcat(field, value);
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == 9 && col == roomWidth+10){
                char field[8] = {'A','M','M','O',':',BLANK};
                char value[3]; //fino a 99
                _itoa(protagonist.getBullet(), value, 10);
                strcat(field, value);
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else { 
                CIview[count].Char.AsciiChar = BLANK;
                CIview[count].Attributes = DEF_COLORFOREGROUND;
            }
        }
    }
}

void game::gameOver() {   
    int count = 0;
    for (int row = 0; row < consoleHeight; row++) {
        for (int col = 0; col < consoleWidth; col++, count++) {
            int myRow = 2;
            if (row == myRow++ && col == 3){
                char field[10] = {' ',' ',' ','_','_','_','_','_','_'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            if (row == myRow++ && col == 3){
                char field[28] = {' ',' ','/',' ','_','_','_','_','/','_','_','_',' ','_','_','_','_','_',' ','_','_','_',' ',' ','_','_','_'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            if (row == myRow++ && col == 3){
                char field[29] = {' ','/',' ','/',' ','_','_','/',' ','_','_',' ','`','/',' ','_','_',' ','`','_','_',' ','K','/',' ','_',' ','K'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            if (row == myRow++ && col == 3){
                char field[29] = {'/',' ','/','_','/',' ','/',' ','/','_','/',' ','/',' ','/',' ','/',' ','/',' ','/',' ','/',' ',' ','_','_','/'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            if (row == myRow++ && col == 3){
                char field[28] = {'K','_','_','_','_','/','K','_','_',',','_','/','_','/',' ','/','_','/',' ','/','_','/','K','_','_','_','/'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }

            if (row == myRow++ && col == 3){
                char field[8] = {' ',' ',' ','_','_','_','_'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            if (row == myRow++ && col == 3){
                char field[25] = {' ',' ','/',' ','_','_',' ','K','_',' ',' ',' ','_','_','_','_','_',' ',' ','_','_','_','_','_'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            if (row == myRow++ && col == 3){
                char field[25] = {' ','/',' ','/',' ','/',' ','/',' ','|',' ','/',' ','/',' ','_',' ','K','/',' ','_','_','_','/'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            if (row == myRow++ && col == 3){
                char field[21] = {'/',' ','/','_','/',' ','/','|',' ','|','/',' ','/',' ',' ','_','_','/',' ','/'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            if (row == myRow++ && col == 3){
                char field[19] = {'K','_','_','_','_','/',' ','|','_','_','_','/','K','_','_','_','/','_','/'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else { 
                CIview[count].Char.AsciiChar = BLANK;
                CIview[count].Attributes = DEF_COLORFOREGROUND;
            }
        }
    }
}