#include "Game.hpp"
#include "conio.h"

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

    //currentroom.initializeView();
    currentroom = new room();
    currentroom->roomGenerator();
    score = 0;
}

//Quando il personaggio arriva in fondo creiamo una nuova room 
//se la stanza è già stata creata bisogna respawnarla
void game::nextRoom(){
        currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = ' ';
        if (currentroom->next==NULL){
            currentroom->next = new room();
            currentroom->next->prev = currentroom;
            currentroom->next->roomNum = currentroom->getRoomNum() + 1;
            currentroom->nextLevelPos();
            currentroom->next->roomGenerator();

            currentroom = currentroom->next;
            //stampView();
        }
        else {
            currentroom = currentroom->next;
            currentroom->nextLevelPos();
            currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = protagonist.getFigure();
            //stampView();
        }
}

//Quando il personaggio vuole tornare indietro
void game::prevRoom(){
    if (currentroom->prev != NULL){
        currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = ' ';
        currentroom = currentroom->prev;
        currentroom->prevLevelPos();
        currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = protagonist.getFigure();
    }
}

void game::stampView() {    
    WriteConsoleOutputA(hStdout, currentroom->CIview, characterBufferSize, characterPosition, &consoleSize);
}

/*
//Funzione per gestire spostamento, cambio stanza, impatto e nemici
*/
void game::logic(){
    if(kbhit()){
        move(getch());
        if(protagonist.getColPos() == roomWidth - 1 && protagonist.getRowPos() == roomHeight-2)
            nextRoom(); 
        if (protagonist.getColPos() == 0 && protagonist.getRowPos() == roomHeight-2)
            prevRoom(); 
        
        //stamp view aggiungere controllo per alive e taken
    }
    currentroom->enemyMove();
    Sleep(200);
}

//Il caso A è il più commentato perchè stato fatto per primo. Per dubbi riferirsi a quello
void game::move(char input){
    switch(input){
        /*Movimento Sopra e Sotto
         */
        case 'w': 
        case 'W': if(currentroom->view[protagonist.getRowPos()-1][protagonist.getColPos()] != ' ' &&    //##quali sono i casi in cui il protagonista non può muoversi? Forse è più facile fare i casi in cui può muoversi
                     currentroom->view[protagonist.getRowPos()-1][protagonist.getColPos()] != '#'){
                        currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = ' ';
                        protagonist.setRowPos(protagonist.getRowPos()-2);                               //##sale di piattaforma -> -2
                        checkCollision(protagonist.getRowPos(), protagonist.getColPos());
                        currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = protagonist.getFigure();
                    }  
            break;
        case 's': 
        case 'S': if(currentroom->view[protagonist.getRowPos()+3][protagonist.getColPos()] != ' ' &&
                     currentroom->view[protagonist.getRowPos()+1][protagonist.getColPos()] != '#'){
                        currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = ' ';
                        protagonist.setRowPos(protagonist.getRowPos()+2);
                        checkCollision(protagonist.getRowPos(), protagonist.getColPos());
                        currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = protagonist.getFigure();
                    }  
            break;
        /*Movimento Sinistra e Destra*/    
        case 'a': 
        case 'A': if(currentroom->view[protagonist.getRowPos()+1][protagonist.getColPos()-1] != ' '){
                        currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = ' ';
                        //change into new value
                        protagonist.setColPos(protagonist.getColPos()-1);
                        //controlliamo la posizione su cui ci spostiamo
                        checkCollision(protagonist.getRowPos(), protagonist.getColPos());
                        //update view
                        currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = protagonist.getFigure();  
                    }
            break;
        case 'd': 
        case 'D': if(currentroom->view[protagonist.getRowPos()+1][protagonist.getColPos()+1] != ' '){
                        currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = ' ';
                        protagonist.setColPos(protagonist.getColPos()+1);
                        checkCollision(protagonist.getRowPos(), protagonist.getColPos());
                        currentroom->view[protagonist.getRowPos()][protagonist.getColPos()] = protagonist.getFigure();
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
    itemNode* iter = currentroom->currentBonus;
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
///////////////////////////////////////
void game::checkCollision (int row, int col){
    if(currentroom->view[row][col] != ' '){
        bool found = false;
        //caso collisione enemy
        if(currentroom->view[row][col] == 'M'){
            protagonist.decreaseLife();
            setScore(-10);
            //dobbiamo muoverci nella lista per vedere con quale nemico ci siamo scontrati
            enemyNode* iter = currentroom->currentMonsters;
            while(!found){
                if(iter->monster.getRowPos() == row && iter->monster.getColPos() == col){
                    iter->monster.setAlive();
                    found = true;
                }
                iter = iter->next;
            }
        }
        //caso item
        //Se non era un mostro dobbiamo controllare quale bonus si trovava in quella posizione
        else if(!found){
            if(currentroom->view[row][col] == 'A'){
                int value = findItem(row, col);
                protagonist.setLife(protagonist.getLife() + value);
            }
            else if(currentroom->view[row][col] == 'B'){
                int value = findItem(row, col);
                protagonist.setBullet(protagonist.getBullet() + value);           
            }
            else if (currentroom->view[row][col] == 'C'){
                int value = findItem(row, col);
                setScore(value);
            }
        }
    }
}
