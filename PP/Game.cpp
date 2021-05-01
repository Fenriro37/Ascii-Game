#include "Game.hpp"

game::game(){
    room firstRoom;
    roomList.myRoom = firstRoom;
    roomList.prev = NULL;
    roomList.next = NULL;

    firstRoom.roomNum = 1;
    firstRoom.initializeView();
    //for (int i = 0; i < arrayLength; i++) {
    //array[i] = newValue[i];}

    firstRoom.view[2][2] = 'S';
    firstRoom.view[2][3] = 't';
    firstRoom.view[2][4] = 'a';
    firstRoom.view[2][5] = 'r';
    firstRoom.view[2][6] = 't';
    
    //firstRoom.initializeView();

    /* Console stuff
        ############################
    */

   _CONSOLE_CURSOR_INFO cursorInfo = {100, false};  //disabilito il cursore con il secondo parametro
   _SMALL_RECT smallRect = {0, 0, consoleWidth - 1, consoleHeight - 1}; //l, t, r, b
   _COORD coordBottomRight = {consoleWidth - 1, consoleHeight - 1}, coordTopLeft = {0, 0};
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);      //set degli handle
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleCursorInfo(hStdout, &cursorInfo);     //modifiche applicate
    SetConsoleScreenBufferSize(hStdout, coordBottomRight);   //coordinata
    SetConsoleWindowInfo(hStdout, true, &smallRect);    //finestra
    //SetCurrentConsoleFontEx(...);    //font del carattere

    controllare char_info 
    e come scrivere sulla console
}