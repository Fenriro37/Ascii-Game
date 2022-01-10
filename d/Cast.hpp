#pragma once
class cast{    
protected:
    int rowPos;
    int colPos;
    char figure;    
    
public:
    cast();
    cast(int x, int y);
    void setRowPos(int newRowpos);
    int getRowPos();
    void setColPos(int newColpos);
    int getColPos();
    int getPos();
    void setFigure(char newFigure);
    char getFigure(); 
};
