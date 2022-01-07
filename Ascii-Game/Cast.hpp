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
    int getPos();
    /*
    int getPrevRowPos();
    int getPrevColPos();
    */
    char getFigure(); 
};
