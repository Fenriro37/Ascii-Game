#include "Room.hpp"
#include <iostream>

void room::initializeView() {

    //ciclo sulle righe   
    for (int row = 0; row < roomHeight; row++) {
        //ciclo sulle colonne
        for (int col = 0; col < roomWidth; col++) {
            //caso muri sinistro e destro
            if (col == 0 ) {
                view[row][col] = '#';
            }
            //caso tetto e pavimento
            else if (row == 0 || row == roomHeight - 1) {
                view[row][col] = '#';
            }
            //player
            else if (row == 13 && col == 1) {         //posizioni inserite manualmente per non aggiungere Cast.hpp
                view[row][col] = '$';
            }
            //enemy
            else if (row == 8 && col == 11) {         //posizioni inserite manualmente per non aggiungere Cast.hpp
                view[row][col] = 'M';
            }
            //resto
            else {
                view[row][col] = ' ';
            }
        }
    }
    toCharInfo();
}

void room::toCharInfo() {
    int count = 0;
    for (int row = 0; row < consoleHeight; row++) {
            for (int col = 0; col < consoleWidth; col++) {
                if (col < roomWidth  && row < roomHeight) {
                    CIview[count].Char.UnicodeChar = view[row][col];
                    CIview[count].Attributes = FOREGROUND_GREEN;
                }
                else { 
                    CIview[count].Char.UnicodeChar = ' ';
                    CIview[count].Attributes = FOREGROUND_GREEN | BACKGROUND_RED;
                }
                count++;
            }
        }
    }

