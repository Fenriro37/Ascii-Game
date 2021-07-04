#include "Game.hpp"
#include "conio.h"

extern cast hero;

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

    /* Set the window size */
    SetConsoleWindowInfo(hStdout, TRUE, &consoleSize);

    /* Set the screen's buffer size */
    SetConsoleScreenBufferSize(hStdout, bufferSize);

    currentroom.initializeView();

}

//quando la x del personaggio arriva in fondo nuova room 
//creata random e dinamicamente e aggiustare i puntatori
void game::generateroom(){
    
    if(hero.XPos == roomWidth - 1){
        room test;
        currentroom.next = &test;
        test.next = NULL;
        test.prev = &currentroom;
        //decidere se creare costruttore per room o creare funzione
        //create room- populate room
    }

}

void game::stampView() {
    
    WriteConsoleOutputA(hStdout, currentroom.CIview, characterBufferSize, characterPosition, &consoleSize);
    //DWORD written = 0;
    //WriteConsoleOutput(hNewScreenBuffer, roomList.myRoom.CIview , roomHeight*roomWidth, &written, NULL);
}

void game::move(){
    //dentro una macro funzione per gestire i movimenti if(kbhit())


}


