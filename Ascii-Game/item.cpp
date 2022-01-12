#include "item.hpp"
#include "Variables.hpp"
item::item(){
    taken = false;
    int random = rand()%10;
    if (random<4) {
        cast::setFigure(HEART);   //♥
    }
    else if (random<8){
        cast::setFigure(MAGAZINE); //®
    }
    else {
        cast::setFigure(COIN); //©
    }
}

//Non abbiamo un metodo per passare da true a false perchè dopo che un bonus è stato preso non sarà
//più prendibile

void item::setTaken(){
    taken = true;
}

bool item::getTaken(){
    return taken;
}