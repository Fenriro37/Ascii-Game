#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

const int Y_DIM = 15;
const int X_DIM = 30;

void initializeArray() {
    char map[Y_DIM][X_DIM];
        
    for (int row = 0; row < Y_DIM; row++)
    {
        for (int col = 0; col < X_DIM; col++)
        {   
            if( col == 0 || col == X_DIM-1)
            {
            map[row][col]='-';
            cout << map[row][col];
            }

            else {
            map[row][col]=' ';
            cout << map[row][col];
            }
        }
    cout << "\n";   
    }
}

int main() {
    initializeArray();
    while(1);
    return 0;
}