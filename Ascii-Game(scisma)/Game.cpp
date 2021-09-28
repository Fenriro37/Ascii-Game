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
    //currentroom->myRoom.roomGenerator();
    score = 0;
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

/*Controlla che la posizione sia uguale a figure
*/
bool game::checkNear(int row, int col, char figure){
    char* PtoArray = currentroom->myRoom.getView();
    PtoArray += roomWidth*row+col;
    return (*PtoArray != figure);
}

void game::nextRoom(){
        changeCellOfView(protagonist.getPos(), BLANK);
        if (currentroom->next == NULL){
            currentroom->next = new roomList(currentroom->myRoom.getRoomNum() + 1);
            currentroom->next->prev = currentroom;
            //currentroom->next->myRoom.setRoomNum(currentroom->myRoom.getRoomNum() + 1);
            
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
    currentroom->myRoom.bulletMove();
    if(kbhit()){
        move(getch());
        if(protagonist.getColPos() == roomWidth - 1 && protagonist.getRowPos() == roomHeight-2)
            nextRoom(); 
        if (protagonist.getColPos() == 0 && protagonist.getRowPos() == roomHeight-2)
            prevRoom(); 
    }
    Sleep(50);
    currentroom->myRoom.bulletMove();
    currentroom->myRoom.enemyMove();

    toCharInfo();
    stampView();
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
        case 'k': //proiettile destra
        case 'K': if(protagonist.getBullet() > 0  && checkNear(protagonist.getRowPos(), protagonist.getColPos()+1, WALL) && 
                     roomWidth*protagonist.getRowPos()+protagonist.getColPos()+1 != roomWidth*(roomHeight-2)+roomWidth-1){
                    protagonist.decreaseBullet();
                    currentroom->myRoom.generateBullet(RIGHT, protagonist);
                  }                  
            break;

        default:    
            break;
    }
}

int game::getScore(){
    return score;
}

void game::setScore(int newScore){
    if (newScore < 0){
        if(score+newScore > 0)
            score=score+newScore;
        else 
            score = 0;
    }
}

int game::findItem(int row, int col){
    itemNode* iter = currentroom->myRoom.getCurrentBonus();
    ////////////////////////////////////////////////
    //while true non troppo carino ma variabile booleana non ha senso
    //decidere quale usare
    ///////////////////////////////////////////////
    while(true){
        if(iter->Bonus.getRowPos() == row && iter->Bonus.getColPos() == col){
            iter->Bonus.setTaken();
            return iter->Bonus.getValue();
        }
        iter = iter->next;
        }
}

////////////////////////////////////////
//Funzione per controllare l'interazione con mostri e items
//Gestisce le vite e lo score
//Aggiornare con le collisioni dei proiettili o aggiungere un'altra funzione
//Funzioni findmMonster() e findBonus() da utilizzare al posto del while
///////////////////////////////////////
void game::playerCollision(int row, int col){
    if(checkNear(row, col, BLANK)){
        //caso collisione enemy
        if(!checkNear(row, col, MONSTER)){
            protagonist.decreaseLife();
            setScore(-10);
            //dobbiamo muoverci nella lista per vedere con quale nemico ci siamo scontrati
            enemyNode* iter;
            iter = currentroom->myRoom.findMoster(row, col);
            iter->monster.setAlive();
        }
        //caso bullet //aggiungere proiettile nemico
        else if(!checkNear(row, col, BULLET)){
            bulletNode* iter;
            iter = currentroom->myRoom.findAmmo(row, col);
            iter->ammo.setAlive();
            protagonist.decreaseLife();
        }
        //caso item
        //Se non era un mostro dobbiamo controllare quale bonus si trovava in quella posizione
        else{
            if(!checkNear(row, col, HEART)){
                int value = findItem(row, col);
                protagonist.setLife(protagonist.getLife() + value);
            }
            else if(!checkNear(row, col, MAGAZINE)){
                int value = findItem(row, col);
                protagonist.setBullet(protagonist.getBullet() + value);           
            }
            else if(checkNear(row, col, COIN)){
                int value = findItem(row, col);
                setScore(value);
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
                _itoa(score, value, 10);
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

