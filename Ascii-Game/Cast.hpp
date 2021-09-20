#pragma once
#include <iostream>

using namespace std;

class cast{
    
    protected:
    int rowPos;
    int colPos;
    /*idea precedente di aggiornare con la posizione precedente l'array view
    per ora scartata. I set e i get ci sono ma probabilmente saranno da rimuovere
    int prevRowPos, prevColPos;
    */
    char figure;
    
    public:
    cast();
    cast(int x, int y);
    void setRowPos(int newRowpos);
    void setColPos(int newColpos);
    /*
    void setPrevRowPos(int currentRowPos);
    void setPrevColPos(int currentColPos);
    */
    void setFigure(char newFigure);
    /*
    void increaseRow();
    void increaseCol();
    void decreaseRow();
    void decreaseCol();
    */
    int getRowPos();
    int getColPos();
    /*
    int getPrevRowPos();
    int getPrevColPos();
    */
    char getFigure(); 
};
