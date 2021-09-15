#pragma once
#include "Cast.hpp"

class item : public cast{
    protected:
    int value; //Variabile per indicare quanti punti varrà il bonus
    bool taken;

    public:
    //vari costruttori per vari bonus?
    item();
    item(int bonus);
    void setValue(int newValue);
    void setTaken();
    int getValue();
    bool getTaken();
};

