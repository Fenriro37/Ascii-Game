#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Map.hpp"

using namespace std;
            
//const int width = 20;
//const int height = 20;
//hero Bugo(1,1,1,1,1);
//char level[height][width];
extern void initializeArray();
extern void move(char mossa);

int main(){
    
    initializeArray();
    while(1)
    {

    char mossa = getch(); //salva il primo tasto che premi da tastira
    move(mossa);           //che passiamo a move
    system("cls");          //refresh dello
    initializeArray();
    }
    return 0;
}