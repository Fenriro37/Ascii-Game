#include <iostream>
#include <conio.h>
#include "Room.hpp"
#include "Game.hpp"

using namespace std;

int main() {
    srand(time(0));
    game x;
    char mossa;
    while (1) {
        x.logic();
        x.toCharInfo();
        x.stampView();

        //x.currentroom.initializeView();
       
    }
    return 0;
}