#pragma once
#include "Cast.hpp"

class item : public cast{
protected:
    bool taken;

public:
    //vari costruttori per vari bonus?
    item();
    void setTaken();
    bool getTaken();
};

