#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Hero.hpp"
//#include "Map.hpp"

using namespace std;

            
//const int width = 20;
//const int height = 20;
//hero Bugo(1,1,1,1,1);
//char level[height][width];
   const int width = 20;
   const int height = 20;
   hero Bugo(1,1,1,1,1);

//class map{
  //  private:
        char level[height][width];
    


void initializeArray() {  
    //ciclo sulle righe    
    for (int row = 0; row < width; row++)
    {   
        //ciclo sulle colonne
        for (int col = 0; col < height; col++)
        {   
            //caso muri sinistro e destro
            if( col == 0 || col == width-1)
            {
                level[row][col]='#';
                cout << level[row][col];
            }   
            //caso tetto e pavimento
            else if(row == 0 || row == width-1 )
            {
                level[row][col] = '#';
                cout<< level[row][col];
            }
            //player
            else if(row==Bugo.Yhero && col==Bugo.Xhero)
            {
                level[row][col] = '$';
                cout<<level[row][col];
            }
            //resto
            else
            {
                level[row][col]=' ';
                cout << level[row][col];
            }
        }
        cout << "\n";   
    }
}

void move(char mossa){
    //switch = serie di if a cascata
    switch(mossa){
        case 'W': //Se premi W il codice parte da qua e scende finchè non viene gestito
        case 'w':{ //in questo caso anche se hai premuto W eseguirai il codice sottostante
            if(level[Bugo.Yhero-1][Bugo.Xhero]!='#')
            Bugo.Yhero--; //in un array bidimensionale il movimento verticale è invertito 
                          //(per andare in alto decremento)
            break; //perchè vogliamo che un solo caso gestisca il problema
        }
        case 'S':
        case 's':{ 
            if(level[Bugo.Yhero+1][Bugo.Xhero]!='#')
            Bugo.Yhero++;
            break;
        }
        case 'A':
        case 'a':{ 
            if(level[Bugo.Yhero][Bugo.Xhero-1]!='#')
            Bugo.Xhero--;
            break;
        }
        case 'D':
        case 'd':{ 
            if(level[Bugo.Yhero][Bugo.Xhero+1]!='#')
            Bugo.Xhero++;
            break;
        }
    }
            
}


int main(){
    
    initializeArray();
    while(1)
    {
    char mossa = getch(); //getch() salva il primo tasto che premi da tastira
    move(mossa);           //che passiamo a move
    system("cls");          //refresh dello
    initializeArray();
    //Sleep(100);
    }
    return 0;
}