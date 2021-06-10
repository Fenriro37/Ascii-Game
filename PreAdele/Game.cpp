#include "Game.hpp"

game::game(){
    room firstRoom;
    roomList.myRoom = firstRoom;
    roomList.prev = NULL;
    roomList.next = NULL;

    firstRoom.roomNum = 1;
    
    /* Console stuff
        ############################
    */

    /* Window size coordinates, be sure to start index at zero! */
    consoleSize = {0, 0, consoleWidth - 1, consoleHeight - 1};

    /* A COORD struct for specificying the console's screen buffer dimensions */
    COORD bufferSize = {consoleWidth, consoleHeight};
    
    /* Setting up different variables for passing to WriteConsoleOutput */
    characterBufferSize = {consoleWidth, consoleHeight};
    characterPosition = {0, 0};
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

    
    firstRoom.initializeView();
    
}

void game::stampView() {
    WriteConsoleOutputA(hStdout, roomList.myRoom.CIview, characterBufferSize, characterPosition, &consoleSize);
    //DWORD written = 0;
    //WriteConsoleOutput(hNewScreenBuffer, roomList.myRoom.CIview , roomHeight*roomWidth, &written, NULL);
}