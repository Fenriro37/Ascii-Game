#pragma once
#include "Cast.hpp"

class item : public cast{
protected:
    bool taken;

public:
    item();
    void setTaken();
    bool getTaken();
};

