#include "Game.hpp"
#include "conio.h"
#include "string.h"
extern hero protagonist;

// WORDS colori
//WORD DEF_COLORFOREGROUND = FOREGROUND_GREEN;
WORD DEF_COLORFOREGROUND = FOREGROUND_GREEN | FOREGROUND_RED;
WORD RED = FOREGROUND_RED;
WORD WHITE = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;

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

    CONSOLE_FONT_INFOEX font = {sizeof(CONSOLE_FONT_INFOEX)};   //CONSOLE_FONT_INFOEX is defined in some windows header
    GetCurrentConsoleFontEx(hStdout, false, &font); //PCONSOLE_FONT_INFOEX is the same as CONSOLE_FONT_INFOEX*
    font.dwFontSize.X = 24;
    font.dwFontSize.Y = 35;
    SetCurrentConsoleFontEx(hStdout, false, &font);

    currentroom = new roomList(0);
    
    toCharInfo();

    changeCellOfView( startRowPos,startColPos, HERO);
    stampView();
}

/*Otteniamo la posizione della cella da cambiare e la sostituiamo con figure
* Funziona perchè gli array sono memoria contigua quindi spostandoci di indirizzo 
* raggiungiamo la cella corretta 
*/
void game::changeCellOfView(int x, int y, char figure){
    CIview[consoleWidth*x+y].Char.AsciiChar = figure;
    CIview[consoleWidth*x+y].Attributes = DEF_COLORFOREGROUND;
}


//Controlla che la posizione sia uguale a figure
//ritorn a true se sono diversi
bool game::checkFigure(int row, int col, char figure){
    //in precedenza c'era roomWidth non ho idea del perchè funzionasse
    return (CIview[consoleWidth *row+col].Char.AsciiChar != figure);
}

void game::nextRoom(){
    //clearList();

    changeCellOfView(protagonist.getRowPos(), protagonist.getColPos(), BLANK);
    if (currentroom->next == NULL){
        protagonist.setScore(50);   //+50 pti per il cambio stanza
        currentroom->next = new roomList(currentroom->myRoom.getRoomNum() + 1);
        currentroom->next->prev = currentroom;

        currentroom = currentroom->next;
    }
    else {
        currentroom = currentroom->next;
        currentroom->myRoom.nextLevelPos();  
    }
    toCharInfo();
    changeCellOfView( startRowPos,startColPos, HERO);
    stampView();
}

//Quando il personaggio vuole tornare indietro
void game::prevRoom(){
    
    //clearList();
    if (currentroom->prev != NULL){
        changeCellOfView(protagonist.getRowPos(), protagonist.getColPos(), BLANK);
        currentroom = currentroom->prev;
        currentroom->myRoom.prevLevelPos();
        toCharInfo();
        changeCellOfView( endRowPos, endColPos, HERO);
        stampView();
    }
}

void game::stampView() {    
    WriteConsoleOutputA(hStdout, CIview, characterBufferSize, characterPosition, &consoleSize);
}

void game::bulletMove(){
    int offSet;
    bulletNode* iter = currentroom->myRoom.getCurrentAmmo();

    while(iter != NULL){
        int position = consoleWidth * iter->ammo.getRowPos() + iter->ammo.getColPos();
       
        if(iter->ammo.getAlive()){
            offSet = (iter->ammo.getDirection() == LEFT) ? -1 : 1;
            int posPlusOffset = consoleWidth * iter->ammo.getRowPos() + iter->ammo.getColPos() +  offSet; 
            //caso muro 
            if( !checkFigure(iter->ammo.getRowPos(), iter->ammo.getColPos() + 1 , WALL) ){
                iter->ammo.setAlive();
                CIview[position].Char.AsciiChar = BLANK;
            }
            //caso proiettile appena generato
            else if ( !checkFigure(iter->ammo.getRowPos(), iter->ammo.getColPos(), HERO)){
                iter->ammo.setColPos(iter->ammo.getColPos() + offSet);
                position += offSet;
                posPlusOffset += offSet;
                CIview[position].Char.AsciiChar = iter->ammo.getFigure();
                stampView();
                Sleep(35);
                CIview[position].Char.AsciiChar = BLANK;

                if(currentroom->myRoom.bulletCollision(iter->ammo.getRowPos(), iter->ammo.getColPos() + offSet) || !checkFigure(iter->ammo.getRowPos(), iter->ammo.getColPos() + 1 , WALL)){
                    iter->ammo.setAlive();                
                }
                else{
                iter->ammo.setColPos(iter->ammo.getColPos() + offSet);
                CIview[posPlusOffset].Char.AsciiChar= iter->ammo.getFigure();
                }
            }
            else if ( !checkFigure(iter->ammo.getRowPos(), iter->ammo.getColPos(), TURRET)){
                iter->ammo.setColPos(iter->ammo.getColPos() + offSet);
                CIview[posPlusOffset].Char.AsciiChar = iter->ammo.getFigure();
            }
            //caso collisione con nemici, bonus, hero e proiettili
            else if(currentroom->myRoom.bulletCollision(iter->ammo.getRowPos(), iter->ammo.getColPos() + offSet)){
                iter->ammo.setAlive(); 
                CIview[position].Char.AsciiChar = BLANK;                 
            }
            //Proiettile si muove di uno
            else{
                CIview[position].Char.AsciiChar = BLANK;
                iter->ammo.setColPos(iter->ammo.getColPos() + offSet);
                CIview[posPlusOffset].Char.AsciiChar = iter->ammo.getFigure();
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

                int posBelow = consoleWidth * iter->monster.getRowPos()+1 + iter->monster.getColPos() + offSet;
                int posPlusOffset = consoleWidth * iter->monster.getRowPos() + iter->monster.getColPos() + offSet;

                if( ( !checkFigure(iter->monster.getRowPos()+1, iter->monster.getColPos() + offSet, BLANK))
                    && ( !checkFigure(iter->monster.getRowPos(), iter->monster.getColPos() + offSet, WALL))
                    && ( !checkFigure(iter->monster.getRowPos(), iter->monster.getColPos() + offSet, MONSTER)) 
                    && consoleWidth * iter->monster.getRowPos() + iter->monster.getColPos()+ offSet != consoleWidth * roomHeight-2
                    && consoleWidth * iter->monster.getRowPos() + iter->monster.getColPos()+ offSet != consoleWidth * roomHeight-2 + roomWidth-1){
                        //serve per rendere BLANK la posizione attuale
                        posPlusOffset -= offSet;
                        if(currentroom->myRoom.enemyCollision(iter)){
                            CIview[posPlusOffset].Char.AsciiChar = BLANK;
                        }
                        else{
                            CIview[posPlusOffset].Char.AsciiChar = BLANK;
                            iter->monster.setColPos(iter->monster.getColPos() + offSet);
                            posPlusOffset += offSet;
                            CIview[posPlusOffset].Char.AsciiChar = iter->monster.getFigure();
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
    while(protagonist.getLife() > 0 && protagonist.getScore() > 0){
        if(kbhit()){
            move(getch());
            if(protagonist.getColPos() == roomWidth - 1 && protagonist.getRowPos() == roomHeight-2)
                nextRoom();
            if (protagonist.getColPos() == 0 && protagonist.getRowPos() == roomHeight-2)
                prevRoom(); 
        }
        Sleep(50);

        bulletMove();
        enemyMove();

        stampView();
    }
    gameOver();
}

//Il caso A è il più commentato perchè stato fatto per primo. Per dubbi riferirsi a quello
void game::move(char input){
    switch(input){
        /*Movimento Sopra e Sotto
         */
        case 'w': 
        case 'W': if(checkFigure(protagonist.getRowPos()-1, protagonist.getColPos(), BLANK) && 
                     checkFigure(protagonist.getRowPos()-1, protagonist.getColPos(), ROOF)){
                        changeCellOfView(protagonist.getRowPos(),protagonist.getColPos(), BLANK);
                        protagonist.setRowPos(protagonist.getRowPos()-2);   //sale di piattaforma -> -2
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos(), 0);
                        changeCellOfView(protagonist.getRowPos(),protagonist.getColPos(), protagonist.getFigure());
                    } 
            break;
        case 's':    //+3 per controllare che ci sia la piattaforma sotto i piedi 
        case 'S': if(checkFigure(protagonist.getRowPos()+3, protagonist.getColPos(), BLANK) &&
                     checkFigure(protagonist.getRowPos()+1, protagonist.getColPos(), FLOOR) ){
                        changeCellOfView(protagonist.getRowPos(),protagonist.getColPos(), BLANK);  
                        protagonist.setRowPos(protagonist.getRowPos()+2);
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos(), 1);
                        changeCellOfView(protagonist.getRowPos(),protagonist.getColPos(), protagonist.getFigure());
                    }
            break;
        /*Movimento Sinistra e Destra*/    
        case 'a': 
        case 'A': if(checkFigure(protagonist.getRowPos()+1, protagonist.getColPos()-1, BLANK) &&
                     checkFigure(protagonist.getRowPos(), protagonist.getColPos()-1, WALL)){
                        changeCellOfView(protagonist.getRowPos(),protagonist.getColPos(), BLANK);
                        //change into new value
                        protagonist.setColPos(protagonist.getColPos()-1);
                        //controlliamo la posizione su cui ci spostiamo
                        //se impattiamo contro porta
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos(), 0);
                        changeCellOfView(protagonist.getRowPos(),protagonist.getColPos(), protagonist.getFigure()); 
                        //altrimente se ci siamo scontrati con nulla o con item o nemici 
                    }
            break;
        case 'd': 
        case 'D': if(checkFigure(protagonist.getRowPos()+1, protagonist.getColPos()+1, BLANK) &&
                     checkFigure(protagonist.getRowPos(), protagonist.getColPos()+1, WALL)){
                        changeCellOfView(protagonist.getRowPos(),protagonist.getColPos(), BLANK);
                        protagonist.setColPos(protagonist.getColPos()+1);
                        playerCollision(protagonist.getRowPos(), protagonist.getColPos(), 0);
                        changeCellOfView(protagonist.getRowPos(),protagonist.getColPos(), protagonist.getFigure()); 
                    } 
            break;
        
        case 'j': //Proiettile sinistra
        case 'J': if(protagonist.getBullet() > 0 && checkFigure(protagonist.getRowPos(), protagonist.getColPos()-1, WALL) && 
                     roomWidth*protagonist.getRowPos()+protagonist.getColPos()-1 != roomWidth*(roomHeight-2)){
                    protagonist.decreaseBullet();
                    currentroom->myRoom.generateBullet(LEFT, protagonist);
                  }
            break;
                //Proiettile destra
        case 'k': if(protagonist.getBullet() > 0  && checkFigure(protagonist.getRowPos(), protagonist.getColPos()+1, WALL) && 
                     roomWidth*protagonist.getRowPos()+protagonist.getColPos()+1 != roomWidth*(roomHeight-2)+roomWidth-1){
                    protagonist.decreaseBullet();
                    currentroom->myRoom.generateBullet(RIGHT, protagonist);
                  }                  
            break;
        case 'p':
        case 'P':   CIview[60].Char.AsciiChar = 'P';
                    CIview[60].Attributes = DEF_COLORFOREGROUND;
                    CIview[61].Char.AsciiChar = 'A';
                    CIview[61].Attributes = DEF_COLORFOREGROUND;
                    CIview[62].Char.AsciiChar = 'U';
                    CIview[62].Attributes = DEF_COLORFOREGROUND;
                    CIview[63].Char.AsciiChar = 'S';
                    CIview[63].Attributes = DEF_COLORFOREGROUND;
                    CIview[64].Char.AsciiChar = 'E';
                    CIview[64].Attributes = DEF_COLORFOREGROUND;
                    stampView();
                    while(1){
                        if(getch() == 'p' || getch() == 'P' ){
                            CIview[60].Char.AsciiChar = BLANK;
                            CIview[61].Char.AsciiChar = BLANK;
                            CIview[62].Char.AsciiChar = BLANK;
                            CIview[63].Char.AsciiChar = BLANK;
                            CIview[64].Char.AsciiChar = BLANK;
                            stampView();
                            break;
                        } 
                    }
                    break;
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
                protagonist.setScore(currentroom->myRoom.getRoomNum() * BONUS_KILL_MULT);   // y=3x bonus da uccisione mostro
            }
            else {
                protagonist.decreaseLife();
                protagonist.setScore(-(currentroom->myRoom.getRoomNum() * COLLISION_DMG_MULT));  // y=4x danno da collisione con mostro
            }            
            //dobbiamo muoverci nella lista per vedere con quale nemico ci siamo scontrati
            enemyNode* iter;
            iter = currentroom->myRoom.findMoster(row, col);
            iter->monster.setAlive();
        }
        //caso bullet 
        else if(!checkFigure(row, col, BULLET)){
            bulletNode* iter;
            iter = currentroom->myRoom.findAmmo(row, col);
            iter->ammo.setAlive();
            protagonist.decreaseLife();
            protagonist.setScore(-(currentroom->myRoom.getRoomNum() * BULLET_DMG_MULT));  // y=2x danno da collisione con proiettile
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
                protagonist.setBullet(protagonist.getBullet() + BULLET_BONUS);
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
            if (col < roomWidth  && row < roomHeight) {
                char* tmp = currentroom->myRoom.getView();
                tmp += roomWidth * row + col;
                CIview[count].Char.AsciiChar = *tmp;
                switch (CIview[count].Char.AsciiChar) {
                    case HEART:
                    case COIN: 
                    case MAGAZINE: CIview[count].Attributes = WHITE; break;
                    default: CIview[count].Attributes = DEF_COLORFOREGROUND; break;
                }
            }
            /*
            * lvl, hp, score, ammo */
            else if (row == 1 && col == roomWidth+10){
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
}

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