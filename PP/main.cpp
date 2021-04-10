#include <iostream>
#include <conio.h>
#include "Room.hpp"
#include "Game.hpp"

using namespace std;


int main(){
    game x;
    while(1)
    {
        char mossa;       //salva il primo tasto che premi da tastira
        mossa = getch();
        cout<<"culo";
        x.roomList.myRoom.move(mossa);           //che passiamo a move
        system("cls");              //refresh dello
        x.roomList.myRoom.initializeView();
        
    }
    return 0;
}