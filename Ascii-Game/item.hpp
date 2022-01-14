#pragma once
#include "Cast.hpp"
#include "Constants.hpp"

class item : public cast{
protected:
    bool taken;

public:
    item();
    void setTaken();
    bool getTaken();
};

