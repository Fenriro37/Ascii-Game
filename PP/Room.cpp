#include "Room.hpp"
    
void room::encounter(){
        if(hero.position == enemy.position){
            hero.life -= enemy.damage;
            enemy.position = -1;
        }
    }

/**/    
void room::initializeView() {
    for (int i = 0; i < roomWidth * roomHeight; i++){
        //caso tetto e pavimento
        if(i>=0 && i<roomWidth /*tetto*/
                || i>= (roomWidth * roomHeight) - roomWidth && i<roomWidth * roomHeight /*pavimento*/){
            view[i].Char.AsciiChar = '#';
        }
        //caso muri sinistro e destro
        else if(i%roomWidth == 0){
            view[i].Char.AsciiChar = '#';
            view[i-1].Char.AsciiChar = '#';
        }
        //player
        else if(i == hero.position){
            view[i].Char.AsciiChar = '$';
        }
        //enemy
        else if(i == enemy.position){
            view[i].Char.AsciiChar = 'M';
        }
        //resto
        else{
            view[i].Char.AsciiChar = ' ';
        }
    }
}

void room::move(char mossa){
    switch(mossa){
        case 'W': //Se premi W il codice parte da qua e scende finchè non viene gestito
        case 'w':{ //in questo caso anche se hai premuto W eseguirai il codice sottostante
            if(view[hero.position - roomWidth].Char.AsciiChar != '#')
            hero.position - roomWidth; //(per andare in alto decremento)
            break; //perchè vogliamo che un solo caso gestisca il problema
        }
        case 'S':
        case 's':{ 
            if(view[hero.position + roomWidth].Char.AsciiChar != '#')
            hero.position + roomWidth;
            break;
        }
        case 'A':
        case 'a':{ 
            if(view[hero.position-1].Char.AsciiChar!='#')
            hero.position--;
            break;
        }
        case 'D':
        case 'd':{ 
            if(view[hero.position+1].Char.AsciiChar!='#')
            hero.position++;
            break;
        }
    }
    encounter();            
}