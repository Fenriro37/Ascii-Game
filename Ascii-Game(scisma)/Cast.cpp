 #include "Cast.hpp" 

/* #Costruttori
*/
cast::cast(){ 
    int rowPos=0;
    int colPos=0;
}
cast::cast(int x, int y){
    rowPos = x;
    colPos = y;
} 


int cast::getRowPos(){
    return rowPos;
}

int cast::getColPos(){
    return colPos;
}

int cast::getPos(){
    return 40 * rowPos + colPos;
}
/*
int cast::getPrevRowPos(){
    return prevRowPos;
}
int cast::getPrevColPos(){
    return prevColPos;
}
*/
char cast::getFigure(){
    return figure;
}
void cast::setRowPos(int newRowPos){
    rowPos = newRowPos;
}
void cast::setColPos(int newColPos){
    colPos = newColPos;
}
/*
void cast::setPrevRowPos(int currentRowPos){
    prevRowPos = currentRowPos;
}
void cast::setPrevColPos(int currentColPos){
    prevColPos = currentColPos;
}
*/
void cast::setFigure(char newFigure){
    figure = newFigure;
}
/*
void cast::increaseRow(){
    rowPos+=2;
}
void cast::increaseCol(){
    colPos+=1;
}
void cast::decreaseRow(){
    rowPos-=2;
}
void cast::decreaseCol(){
    colPos-=1;
}
*/