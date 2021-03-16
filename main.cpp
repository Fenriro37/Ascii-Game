#include <iostream>
//#include "Map.hpp"
using namespace std;

            
const int width = 10;
const int height = 20;


char level[height][width];

void initializeArray() {
            
            for (int row = 0; row < width; row++)
            {
                for (int col = 0; col < height; col++)
                {   
                    if( col == 0 || col == width-1)
                    {
                        level[row][col]='|';
                        cout << level[row][col];
                    }   
                    else if(row == 0 || row == width-1 )
                    {
                        level[row][col] = '_';
                        cout<< level[row][col];
                    }
                    else
                    {
                        level[row][col]=' ';
                        cout << level[row][col];
                    }
                }
                cout << "\n";   
            }
            level[height-1][1] = '$';
            cout<< level[height-1][1];
        }

int main(){

    initializeArray();
    while(1);
    return 0;
}
