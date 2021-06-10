#include "Room.hpp"

void room::initializeView(){
    
    //ciclo sulle righe   
    for (int row = 0; row < roomHeight; row++){   
        //ciclo sulle colonne
        for (int col = 0; col < roomWidth; col++){   
            //caso muri sinistro e destro
            if( col == 0 || col == roomWidth-1){
                view[row][col]='#';
            }   
            //caso tetto e pavimento
            else if(row == 0 || row == roomHeight-1 ) {
                view[row][col] = '#';
            }
            //player
            else if(row==hero.YPos && col==hero.XPos){
                view[row][col] = '$';
            }
            //enemy
            else if(row==enemy.YPos && col==enemy.XPos) {
                view[row][col] = 'M';
            }
            //resto
            else{
                view[row][col]=' ';
            }
        }
    } 
    /*
    for (int i=0; i<roomHeight; i++){
        for (int j=0; j<roomWidth; j++){
            view[i][j] = 'M';
            //cout << view[i][j];
        }
        //cout << endl;
    }*/
    toCharInfo();
}

void room::toCharInfo(){
    int count = 0;
    for (int row=0; row < roomHeight; row++){
        for (int col=0; col < roomWidth; col++){
            CIview[count].Char.UnicodeChar = view[row][col];
            CIview[count++].Attributes = FOREGROUND_GREEN|BACKGROUND_RED;
        }
    }
}