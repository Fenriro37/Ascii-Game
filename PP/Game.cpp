#include "Game.hpp"
#include "string"

game::game(){
    room firstRoom;
    roomList.myRoom = firstRoom;
    roomList.prev = NULL;
    roomList.next = NULL;

    firstRoom.roomNum = 1;
    //firstRoom.initializeView();
    //for (int i = 0; i < arrayLength; i++) {
    //array[i] = newValue[i];}

    firstRoom.view[2][2] = 'S';
    firstRoom.view[2][3] = 't';
    firstRoom.view[2][4] = 'a';
    firstRoom.view[2][5] = 'r';
    firstRoom.view[2][6] = 't';
    
    firstRoom.initializeView();

}



