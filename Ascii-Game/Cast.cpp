#include "Cast.hpp"

cast::cast(){ 
    rowPos=0;
    colPos=0;
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
    return roomWidth * rowPos + colPos;
}
char cast::getFigure(){
    return figure;
}
void cast::setRowPos(int newRowPos){
    rowPos = newRowPos;
}
void cast::setColPos(int newColPos){
    colPos = newColPos;
}
void cast::setFigure(char newFigure){
    figure = newFigure;
}