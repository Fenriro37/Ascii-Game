#include "item.hpp"
#include "Variables.hpp"
item::item(){
    taken = false;
    
    switch (rand()%3)
    {
    case 0:    //1 vita in più
        setValue(1);
        cast::setFigure(HEART);   //♥
        break;
    case 1:   //Più 2 proiettili
        setValue(2);
        cast::setFigure(MAGAZINE); //¤
        break;
    case 2:    //Score più 10 punti
        setValue(10);
        cast::setFigure(COIN); //©
        break;    
    }
}

void item::setValue(int newValue){
    value = newValue;
}

//Non abbiamo un metodo per passare da true a false perchè dopo che un bonus è stato preso non sarà
//più prendibile

void item::setTaken(){
    taken = true;
}

bool item::getTaken(){
    return taken;
}

int item::getValue(){
    return value;
}

