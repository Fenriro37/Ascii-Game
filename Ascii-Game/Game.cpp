#include "Game.hpp"
#include "conio.h"
#include "string.h"
extern hero protagonist;

// WORDS colori
WORD PLAYER =  FOREGROUND_GREEN;
WORD ENEMIES = FOREGROUND_RED | FOREGROUND_GREEN ;
WORD BONUS =  FOREGROUND_GREEN | FOREGROUND_BLUE;
WORD WHITE = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
WORD DEF_COLORFOREGROUND = WHITE;

game::game() {

    /* Dimensione console */
    consoleSize = { 0, 0, consoleWidth - 1, consoleHeight - 1 };
    /*dimensione screen buffer della console */
    COORD bufferSize = { consoleWidth, consoleHeight };
    /* variabili per WriteConsoleOutput */
    characterBufferSize = { consoleWidth, consoleHeight };
    characterPosition = { 0, 0 };
    /* CHAR_INFO: struttura contenente informazioni riguardanti un singolo carattere */
    CHAR_INFO consoleBuffer[consoleWidth * consoleHeight];

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    //Titolo Console
    SetConsoleTitleA("Operation Antarctica");

	//Cursore invisibile
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(hStdout, &cursor);
    cursor.bVisible = false;
	SetConsoleCursorInfo(hStdout, &cursor);

    //Dimensione della console
    SetConsoleWindowInfo(hStdout, TRUE, &consoleSize);

    //Dimensione del buffer
    SetConsoleScreenBufferSize(hStdout, bufferSize);

    CONSOLE_FONT_INFOEX font = {sizeof(CONSOLE_FONT_INFOEX)};   
    GetCurrentConsoleFontEx(hStdout, false, &font); 
    font.dwFontSize.X = 24;
    font.dwFontSize.Y = 35;
    SetCurrentConsoleFontEx(hStdout, false, &font);

    currentroom = new roomList(0);
    toCharInfo();
}

/*Otteniamo la posizione della cella da cambiare e la sostituiamo con figure
* Funziona perchè gli array sono memoria contigua quindi spostandoci di indirizzo 
* raggiungiamo la cella corretta 
*/
void game::changeCellOfView(int position, char figure){
    if(position >= 0 && position<= roomWidth*roomHeight){
        char* PtoArray = currentroom->myRoom.getView();
        PtoArray += position;
        *PtoArray = figure;
    }
}


//Controlla che la posizione sia uguale a figure
//ritorna true se sono diversi
bool game::checkFigure(int row, int col, char figure){
    char* PtoArray = currentroom->myRoom.getView();
    PtoArray += roomWidth*row+col;  //aritmetica dei puntatori applicata agli array 
    return (*PtoArray != figure);
}

void game::nextRoom(){
    changeCellOfView(protagonist.getPos(), BLANK);
    currentroom->myRoom.clearList();
    if (currentroom->next == NULL){
        protagonist.setScore(50);   //+50 pti per il cambio stanza
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

void game::prevRoom(){
    currentroom->myRoom.clearList();
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

void game::bulletMove(){
    int offSet;
    bulletNode* iter = currentroom->myRoom.getCurrentAmmo();

    while(iter != NULL){
        char* position = currentroom->myRoom.getView();
        char* posPlusOffset = currentroom->myRoom.getView();
        position += iter->ammo.getPos();
       
        if(iter->ammo.getAlive()){
            offSet = (iter->ammo.getDirection() == LEFT) ? -1 : 1;
            posPlusOffset += iter->ammo.getPos() +  offSet; 
            //caso muro 
            if(*posPlusOffset == WALL){
                iter->ammo.setAliveFalse();
                *position = BLANK;
            }
            //caso proiettile appena generato
            else if (*position == HERO){
                iter->ammo.setColPos(iter->ammo.getColPos() + offSet);
                position += offSet;
                posPlusOffset += offSet;
                *position = iter->ammo.getFigure();
                toCharInfo();
                Sleep(35);
                *position = BLANK;

                if(currentroom->myRoom.bulletCollision(iter->ammo.getRowPos(), iter->ammo.getColPos() + offSet) || *posPlusOffset == WALL){
                    iter->ammo.setAliveFalse();                
                }
                else{
                    iter->ammo.setColPos(iter->ammo.getColPos() + offSet);
                    *posPlusOffset = iter->ammo.getFigure();
                }
            }
            else if (*position == TURRET){
                iter->ammo.setColPos(iter->ammo.getColPos() + offSet);
                *posPlusOffset = iter->ammo.getFigure();
            }
            //caso collisione con nemici, bonus, hero e proiettili
            else if(currentroom->myRoom.bulletCollision(iter->ammo.getRowPos(), iter->ammo.getColPos() + offSet)){
                iter->ammo.setAliveFalse(); 
                *position = BLANK;                 
            }
            //Proiettile si muove di uno
            else{
                *position = BLANK;
                iter->ammo.setColPos(iter->ammo.getColPos() + offSet);
                *posPlusOffset = iter->ammo.getFigure();
            }
        }
        iter = iter->next;
    }
}

void game::enemyMove(){ 
    enemyNode* iter = currentroom->myRoom.getCurrentMonsters();
    int offSet;
    while(iter != NULL){
        if(iter->monster.getAlive()){
            //caso TURRET
            if(iter->monster.getFigure() == TURRET){
                //caso sparo a sinistra
                if(protagonist.getColPos()<= iter->monster.getColPos() && iter->monster.getFireDelay() == fireRate){
                    currentroom->myRoom.generateBullet(LEFT, iter->monster);
                    iter->monster.resetFireDelay();
                }
                //caso sparo a destra
                else if (iter->monster.getFireDelay()== fireRate){
                    currentroom->myRoom.generateBullet(RIGHT, iter->monster);
                    iter->monster.resetFireDelay();
                }
                //caso attesa
                else{
                    iter->monster.increaseFireDelay();
                }
            }
            //caso MONSTER
            else if(iter->monster.getFigure() == MONSTER){
                //if ternario : se condizione è vera -1, altrimenti 1
                offSet = (iter->monster.getDirection()==LEFT) ? -1 : 1;

                char* posBelow = currentroom->myRoom.getView();
                posBelow +=  currentroom->myRoom.toSingleArray(iter->monster.getRowPos()+1, iter->monster.getColPos() + offSet);
                char* posPlusOffset = currentroom->myRoom.getView();
                posPlusOffset += iter->monster.getPos() + offSet;

                if(*posBelow != BLANK 
                    && *posPlusOffset != WALL
                    && *posPlusOffset != MONSTER
                    && iter->monster.getPos()+ offSet != currentroom->myRoom.toSingleArray(roomHeight-2, 0)
                    && iter->monster.getPos() + offSet != currentroom->myRoom.toSingleArray(roomHeight-2, roomWidth-1)){
                        //serve per rendere BLANK la posizione attuale
                        posPlusOffset -= offSet;
                        if(currentroom->myRoom.enemyCollision(iter)){
                            *posPlusOffset = BLANK;
                        }
                        else{
                            *posPlusOffset = BLANK;
                            iter->monster.setColPos(iter->monster.getColPos() + offSet);
                            posPlusOffset += offSet;
                            *posPlusOffset = iter->monster.getFigure();
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

//Funzione per gestire spostamento, cambio stanza, impatto e nemici
void game::logic(){
    //title();
    while(protagonist.getLife() > 0 && protagonist.getScore() > 0){
        if(currentroom->myRoom.getRoomNum() == 0 && checkFigure(startRowPos, startColPos+ 18, MONSTER)){
            int position = roomWidth * 4 + 22;
            char field[12] = {'j',' ','k',' ','t','o',' ','f','i','r','e', BLANK};
            for(int i = position,j = 0; i<position+12; i++, j++){
                changeCellOfView(i, field[j]);
            }
            toCharInfo();
        }
        if(kbhit()){
            move(getch());
            if(protagonist.getColPos() == roomWidth - 1 && protagonist.getRowPos() == roomHeight-2)
                nextRoom();
            if (protagonist.getColPos() == 0 && protagonist.getRowPos() == roomHeight-2)
                prevRoom(); 
        }
        Sleep(50);

        bulletMove();
        if(currentroom->myRoom.getRoomNum() != 0)enemyMove();

        toCharInfo();
    }
    gameOver();
}

//Il caso A è il più commentato perchè stato fatto per primo. Per dubbi riferirsi a quello
void game::move(char input){
    switch(input){
        /*Movimento Sopra e Sotto
         */
        case 'w': if(checkFigure(protagonist.getRowPos()-1, protagonist.getColPos(), BLANK) && 
                     checkFigure(protagonist.getRowPos()-1, protagonist.getColPos(), ROOF)){
                        changeCellOfView(protagonist.getPos(), BLANK);
                        protagonist.setRowPos(protagonist.getRowPos()-2);   //sale di piattaforma -> -2
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos(), 0);
                        changeCellOfView(protagonist.getPos(), protagonist.getFigure());
                    } 
            break;
        //+3 per controllare che ci sia la piattaforma sotto i piedi 
        case 's': if(checkFigure(protagonist.getRowPos()+3, protagonist.getColPos(), BLANK) &&
                     checkFigure(protagonist.getRowPos()+1, protagonist.getColPos(), FLOOR) ){
                        changeCellOfView(protagonist.getPos(), BLANK);  
                        protagonist.setRowPos(protagonist.getRowPos()+2);
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos(), 1);
                        changeCellOfView(protagonist.getPos(), protagonist.getFigure());
                    }
            break;
        /*Movimento Sinistra e Destra*/    
        case 'a': if(checkFigure(protagonist.getRowPos()+1, protagonist.getColPos()-1, BLANK) &&
                     checkFigure(protagonist.getRowPos(), protagonist.getColPos()-1, WALL)){
                        changeCellOfView(protagonist.getPos(), BLANK);
                        //change into new value
                        protagonist.setColPos(protagonist.getColPos()-1);
                        //controlliamo la posizione su cui ci spostiamo
                        //se impattiamo contro porta
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos(), 0);
                        changeCellOfView(protagonist.getPos(), protagonist.getFigure()); 
                        //altrimente se ci siamo scontrati con nulla o con item o nemici 
                    }
            break;
        case 'd': if(checkFigure(protagonist.getRowPos()+1, protagonist.getColPos()+1, BLANK) &&
                     checkFigure(protagonist.getRowPos(), protagonist.getColPos()+1, WALL)){
                        changeCellOfView(protagonist.getPos(), BLANK);
                        protagonist.setColPos(protagonist.getColPos()+1);
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos(), 0);
                        changeCellOfView(protagonist.getPos(), protagonist.getFigure()); 
                    } 
            break;
        //Proiettile sinistra
        case 'j': if(protagonist.getBullet() > 0 && checkFigure(protagonist.getRowPos(), protagonist.getColPos()-1, WALL) && 
                     roomWidth*protagonist.getRowPos()+protagonist.getColPos()-1 != roomWidth*(roomHeight-2))
                    if(currentroom->myRoom.getRoomNum() == 0 && checkFigure(startRowPos, startColPos+ 18, MONSTER) || currentroom->myRoom.getRoomNum() != 0){ 
                        protagonist.decreaseBullet();
                        currentroom->myRoom.generateBullet(LEFT, protagonist);
                    }
            break;
                //Proiettile destra
        case 'k':   if(protagonist.getBullet() > 0  && checkFigure(protagonist.getRowPos(), protagonist.getColPos()+1, WALL) && 
                        roomWidth*protagonist.getRowPos()+protagonist.getColPos()+1 != roomWidth*(roomHeight-2)+roomWidth-1)
                            if(currentroom->myRoom.getRoomNum() == 0 && checkFigure(startRowPos, startColPos+ 18, MONSTER) || currentroom->myRoom.getRoomNum() != 0){
                                protagonist.decreaseBullet();
                                currentroom->myRoom.generateBullet(RIGHT, protagonist);
                            }                  
            break;
        case 'p': { char pause[] = {'P','A','U','S','E'};
                    for(int i = 60, j = 0; i<65; i++, j++){
                        CIview[i].Char.AsciiChar = pause[j];
                        CIview[i].Attributes = DEF_COLORFOREGROUND;
                    }
                        
                    stampView();
                    while(1){
                        if(getch() == 'p' || getch() == 'P' ){
                            for(int i = 60; i<65; i++){
                                CIview[i].Char.AsciiChar = BLANK;
                            }
                            stampView();
                            break;
                        } 
                    }
                    break;
                }
        default:    
            break;
    }
}

//Funzione per controllare l'interazione con mostri e items
//Gestisce le vite e lo score
void game::playerCollision(int row, int col, int cameFromAbove){
    if(checkFigure(row, col, BLANK)){
        //caso collisione enemy
        if(!checkFigure(row, col, MONSTER) || !checkFigure(row, col, TURRET)){
            //se siamo arrivati da sopra non subiamo danno
            if(cameFromAbove){
                protagonist.setScore(currentroom->myRoom.getRoomNum() * BONUS_KILL_MULT);   
            }
            else {
                protagonist.decreaseLife();
                protagonist.setScore(-(currentroom->myRoom.getRoomNum() * COLLISION_DMG_MULT));  
            }            
            //dobbiamo muoverci nella lista per vedere con quale nemico ci siamo scontrati
            enemyNode* iter;
            iter = currentroom->myRoom.findMoster(row, col);
            iter->monster.setAliveFalse();
        }
        //caso bullet 
        else if(!checkFigure(row, col, BULLET)){
            bulletNode* iter;
            iter = currentroom->myRoom.findAmmo(row, col);
            iter->ammo.setAliveFalse();
            protagonist.decreaseLife();
            protagonist.setScore(-(currentroom->myRoom.getRoomNum() * BULLET_DMG_MULT));  
        }
        //caso item
        //Se non era un mostro dobbiamo controllare quale bonus si trovava in quella posizione
        else{
            itemNode* currentItem;
            if(!checkFigure(row, col, HEART)){
                currentItem = currentroom->myRoom.findBonus(row, col);
                currentItem->Bonus.setTaken();
                protagonist.increaseLife();
            }
            else if(!checkFigure(row, col, MAGAZINE)){
                currentItem = currentroom->myRoom.findBonus(row, col);
                currentItem->Bonus.setTaken();
                protagonist.setBullet(BULLET_BONUS);
            }
            else if(!checkFigure(row, col, COIN)){
                currentItem = currentroom->myRoom.findBonus(row, col);
                currentItem->Bonus.setTaken();
                protagonist.setScore(currentroom->myRoom.getRoomNum() * COIN_MULT); 
            }
        }
    }
}

//copia l'array passato come argomento aggiornando le variabili passate per indirizzo
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
            //Room
            if (col < roomWidth  && row < roomHeight) {
                char* tmp = currentroom->myRoom.getView();
                tmp += roomWidth * row + col;
                CIview[count].Char.AsciiChar = *tmp;
                switch (CIview[count].Char.AsciiChar) {
                    case HEART:
                    case COIN: 
                    case MAGAZINE: CIview[count].Attributes = BONUS; break;
                    case BULLET:
                    case MONSTER:
                    case TURRET: CIview[count].Attributes = ENEMIES ; break;
                    case HERO: CIview[count].Attributes = PLAYER ; break;
                    default: CIview[count].Attributes = DEF_COLORFOREGROUND; break;
                }
            }
            //Console
            else if (row == 1 && col == roomWidth+10){
                char field[9] = {'L','V','L',':',BLANK};
                char value[4];
                /* _itoa() funzione che converte il primo argomento (intero) in stringa
                * e lo salva nel secondo argomento, il terzo argomento è la base di conversione
                */
                _itoa(currentroom->myRoom.getRoomNum(), value, 10);
                strcat(field, value);
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == 3 && col == roomWidth+10){
                char field[13] = {'S','C','O','R','E',':',BLANK};
                char value[6]; //fino a 999.999
                _itoa(protagonist.getScore() , value, 10);
                strcat(field, value);
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == 6 && col == roomWidth+10){
                char field[6] = {'H','P',':',BLANK};
                char value[2]; //fino a 99
                _itoa(protagonist.getLife(), value, 10);
                strcat(field, value);
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == 8 && col == roomWidth+10){
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
    stampView();
}

//funzione che scrive gameover in caso di perdita
void game::gameOver() {   
    int count = 0;
    int myCol = 6;
    for (int row = 0; row < consoleHeight; row++) {
        for (int col = 0; col < consoleWidth; col++, count++) {
            int myRow = 1;
            if (row == myRow++ && col == myCol){
                char field[9] = {' ',' ',' ','_','_','_','_','_','_'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == myRow++ && col == myCol){
                char field[27] = {' ',' ','/',' ','_','_','_','_','/','_','_','_',' ','_','_','_','_','_',' ','_','_','_',' ',' ','_','_','_'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == myRow++ && col == myCol){
                char field[28] = {' ','/',' ','/',' ','_','_','/',' ','_','_',' ','`','/',' ','_','_',' ','`','_','_',' ','\\','/',' ','_',' ','\\'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == myRow++ && col == myCol){
                char field[28] = {'/',' ','/','_','/',' ','/',' ','/','_','/',' ','/',' ','/',' ','/',' ','/',' ','/',' ','/',' ',' ','_','_','/'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == myRow++ && col == myCol){
                char field[27] = {'\\','_','_','_','_','/','\\','_','_',',','_','/','_','/',' ','/','_','/',' ','/','_','/','\\','_','_','_','/'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }

            else if (row == myRow++ && col == myCol){
                char field[7] = {' ',' ',' ','_','_','_','_'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == myRow++ && col == myCol){
                char field[24] = {' ',' ','/',' ','_','_',' ','\\','_',' ',' ',' ','_','_','_','_','_',' ',' ','_','_','_','_','_'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == myRow++ && col == myCol){
                char field[24] = {' ','/',' ','/',' ','/',' ','/',' ','|',' ','/',' ','/',' ','_',' ','\\','/',' ','_','_','_','/'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == myRow++ && col == myCol){
                char field[20] = {'/',' ','/','_','/',' ','/','|',' ','|','/',' ','/',' ',' ','_','_','/',' ','/'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == myRow++ && col == myCol){
                char field[19] = {'\\','_','_','_','_','/',' ','|','_','_','_','/','\\','_','_','_','/','_','/'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == 10 && col == roomWidth+10){
                char field[9] = {'p','r','e','s','s',' ','E','S','C'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
            else if (row == 11 && col == roomWidth+10){
                char field[10] = {'t','o',' ','e','x','i','t','.','.','.'};
                int size = sizeof(field)/sizeof(field[0]);
                paste(field, size, count, col);
            }
        }
    }
    stampView();
    while(1){
        if(getch() == (char)27) break;
    }
}

//funzione schermata iniziale
void game::title(){
    for(int i = 0; i < consoleHeight * consoleWidth ; i++){
        CIview[i].Char.AsciiChar = BLANK;
        CIview[i].Attributes = WHITE;
    }

    for(int i = consoleWidth * 0 + 8, j=0; i < consoleWidth * 0 + 8 + 40; i++, j++){
        char field[40] = {' ',' ',' ',' ',' ',' ',' ','_','_','_','_',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','_','_',' ',' ','_'};
        CIview[i].Char.AsciiChar = field[j];
    }
    for(int i = consoleWidth * 1 + 8, j=0; i < consoleWidth * 1 + 8 + 50; i++, j++){
        char field[50] = {' ',' ',' ',' ',' ',' ','/',' ','_','_',' ','\\','_','_','_','_',' ',' ','_','_','_',' ',' ','_','_','_','_','_','_','_','_','_',' ','_','/',' ','/','_','(','_',')','_','_','_',' ',' ','_','_','_','_'};
        CIview[i].Char.AsciiChar = field[j];
    }
    for(int i = consoleWidth * 2 + 8, j=0; i < consoleWidth * 2 + 8 + 51; i++, j++){
        char field[51] = {' ',' ',' ',' ',' ','/',' ','/',' ','/',' ','/',' ','_','_',' ','\\','/',' ','_',' ','\\','/',' ','_','_','_','/',' ','_','_',' ','`','/',' ','_','_','/',' ','/',' ','_','_',' ','\\','/',' ','_','_',' ','\\'};
        CIview[i].Char.AsciiChar = field[j];
    }
    for(int i = consoleWidth * 3 + 8, j=0; i < consoleWidth * 3 + 8 + 51; i++, j++){
        char field[51] = {' ',' ',' ',' ','/',' ','/','_','/',' ','/',' ','/','_','/',' ','/',' ',' ','_','_','/',' ','/',' ',' ','/',' ','/','_','/',' ','/',' ','/','_','/',' ','/',' ','/','_','/',' ','/',' ','/',' ','/',' ','/'};
        CIview[i].Char.AsciiChar = field[j];
    }
    for(int i = consoleWidth * 4 + 8, j=0; i < consoleWidth * 4 + 8 + 50; i++, j++){
        char field[50] = {' ',' ',' ',' ','\\','_','_','_','_','/',' ','.','_','_','_','/','\\','_','_','_','/','_','/',' ',' ',' ','\\','_','_',',','_','/','\\','_','_','/','_','/','\\','_','_','_','_','/','_','/',' ','/','_','/'};
        CIview[i].Char.AsciiChar = field[j];
    }
    for(int i = consoleWidth * 5 + 8, j=0; i < consoleWidth * 5 + 8 + 11; i++, j++){
        char field[11] = {' ',' ',' ',' ',' ',' ',' ',' ','/','_','/'};
        CIview[i].Char.AsciiChar = field[j];
    }
    for(int i = consoleWidth * 6 + 8, j=0; i < consoleWidth * 6 + 8 + 42; i++, j++){
        char field[42] = {' ',' ',' ',' ','_','_','_',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','_','_',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','_','_',' ',' ','_'};
        CIview[i].Char.AsciiChar = field[j];
    }
    for(int i = consoleWidth * 7 + 8, j=0; i < consoleWidth * 7 + 8 + 53; i++, j++){
        char field[53] = {' ',' ',' ','/',' ',' ',' ','|',' ',' ','_','_','_','_',' ',' ','/',' ','/','_','_','_','_','_',' ','_','_','_','_','_','_','_','_','_','_','_','/',' ','/','_','(','_',')','_','_','_','_','_','_','_','_',' ','_'};
        CIview[i].Char.AsciiChar = field[j];
    }
    for(int i = consoleWidth * 8 + 8, j=0; i < consoleWidth * 8 + 8 + 53; i++, j++){
        char field[53] = {' ',' ','/',' ','/','|',' ','|',' ','/',' ','_','_',' ','\\','/',' ','_','_','/',' ','_','_',' ','`','/',' ','_','_','_','/',' ','_','_','_','/',' ','_','_','/',' ','/',' ','_','_','_','/',' ','_','_',' ','`','/'};
        CIview[i].Char.AsciiChar = field[j];
    }
    for(int i = consoleWidth * 9 + 8, j=0; i < consoleWidth * 9 + 8 + 52; i++, j++){
        char field[52] = {' ','/',' ','_','_','_',' ','|','/',' ','/',' ','/',' ','/',' ','/','_','/',' ','/','_','/',' ','/',' ','/',' ',' ','/',' ','/','_','_','/',' ','/','_','/',' ','/',' ','/','_','_','/',' ','/','_','/',' ','/'};
        CIview[i].Char.AsciiChar = field[j];
    }
    for(int i = consoleWidth * 10 + 8, j=0; i < consoleWidth * 10 + 8 + 51; i++, j++){
        char field[51] = {'/','_','/',' ',' ','|','_','/','_','/',' ','/','_','/','\\','_','_','/','\\','_','_',',','_','/','_','/',' ',' ',' ','\\','_','_','_','/','\\','_','_','/','_','/','\\','_','_','_','/','\\','_','_',',','_','/'};
        CIview[i].Char.AsciiChar = field[j];
    }
    for(int i = consoleWidth * 12 + 8, j=0; i < consoleWidth * 12 + 8 + 37; i++, j++){
        char field[37] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','p','r','e','s','s',' ','A','N','Y',' ','K','E','Y',' ','t','o',' ','s','t','a','r','t','!'};
        CIview[i].Char.AsciiChar = field[j];
        if (i>=consoleWidth * 12+8+20 && i<=consoleWidth * 12+8+27)
            CIview[i].Attributes = FOREGROUND_GREEN;
    }
         
    stampView();
    while(1){
        if(kbhit()) break;
    }
    for(int i = 0; i < consoleHeight * consoleWidth ; i++){
        CIview[i].Char.AsciiChar = BLANK;
    }
    stampView();
}